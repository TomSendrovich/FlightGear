//
// Created by guy on 13/12/2019.
//

#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "OpenServerCommand.h"
#include "../Expressions/Expression.h"
#include "../Expressions/Calculator.h"

#define XML_SIZE 36

using namespace std;
string vars[] = {"airspeed-indicator_indicated-speed-kt",
                 "time_warp",
                 "switches_magnetos",
                 "heading-indicator_offset-deg",
                 "altimeter_indicated-altitude-ft",
                 "altimeter_pressure-alt-ft",
                 "attitude-indicator_indicated-pitch-deg",
                 "attitude-indicator_indicated-roll-deg",
                 "attitude-indicator_internal-pitch-deg",
                 "attitude-indicator_internal-roll-deg",
                 "encoder_indicated-altitude-ft",
                 "encoder_pressure-alt-ft",
                 "gps_indicated-altitude-ft",
                 "gps_indicated-ground-speed-kt",
                 "gps_indicated-vertical-speed",
                 "indicated-heading-deg",
                 "magnetic-compass_indicated-heading-deg",
                 "slip-skid-ball_indicated-slip-skid",
                 "turn-indicator_indicated-turn-rate",
                 "vertical-speed-indicator_indicated-speed-fpm",
                 "flight_aileron",
                 "flight_elevator",
                 "flight_rudder",
                 "flight_flaps",
                 "engine_throttle",
                 "current-engine_throttle",
                 "switches_master-avionics",
                 "switches_starter",
                 "active-engine_auto-start",
                 "flight_speedbrake",
                 "c172p_brake-parking",
                 "engine_primer",
                 "current-engine_mixture",
                 "switches_master-bat",
                 "switches_master-alt",
                 "engine_rpm",
};

OpenServerCommand::OpenServerCommand() = default;
OpenServerCommand::~OpenServerCommand() = default;
int OpenServerCommand::execute(Data* data) {
  //calculating port number as an expression
  string value = data->getTextArr()[_index + 1];
  int portNum;

  auto* interpreter = new Interpreter();
  Expression* expression = nullptr;

  //setting variables for interpreter
  for (pair<string, VarInfo*> element : data->getSymTableUser()) {
    ostringstream temp;
    temp << element.second->getValue();
    string valueStr = temp.str();
    if (value.find(element.second->getName()) != string::npos) {
      string variable = element.second->getName() + "=" + valueStr;
      interpreter->setVariables(variable);
    }
  }

  try {
    expression = interpreter->interpret(value);
    portNum = (int) expression->calculate();
    delete expression;
    delete interpreter;
  } catch (const char* message) {
    cout << message << endl;
    delete expression;
    delete interpreter;
  }

  try {
    openServer(portNum, data->getSymTableUser(), data->getSymTableSimulator());
  } catch (const char* message) { cout << message << endl; }
  return 2;
}
void OpenServerCommand::openServer(int portNum,
                                   unordered_map<string, VarInfo*>& symTableUser,
                                   unordered_map<string, VarInfo*>& symTableSimulator) {
  //create socket
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) { throw "ERROR: cannot create a socket"; }
  //bind socket to IP address
  sockaddr_in address; //in means IP4
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
  address.sin_port = htons(portNum);

  //the bind command
  if (bind(socketfd, (struct sockaddr*) &address, sizeof(address)) == -1) {
    throw "ERROR: cannot bind to the IP address";
  }

  //making socket listen to the port
  if (listen(socketfd, 1) == -1) { //can also set to SOMAXCON (max connections)
    throw "ERROR: cannot make socket listen to the port";
  } else { cout << "Server is now listening ..." << endl; }

  // accepting a client
  socklen_t size = sizeof(address);
  int client_socket = accept(socketfd, (struct sockaddr*) &address, &size);

  if (client_socket == -1) { throw "ERROR: cannot accept client"; }

  close(socketfd); //closing the listening socket

  //reading from client
  thread newServer(runningServer, client_socket, ref(symTableUser), ref(symTableSimulator));
  newServer.detach();
}
void OpenServerCommand::runningServer(int client_socket,
                                      unordered_map<string, VarInfo*>& symTableUser,
                                      unordered_map<string, VarInfo*>& symTableSimulator) {
  while (true) {
    //reading buffer and sent it to a parse function
    char buffer[1024] = {0};
    int valRead = read(client_socket, buffer, 1024);
    if (valRead != 0 && valRead != -1) { parseSimulatorInput(buffer, symTableUser, symTableSimulator); }
  }
}
void OpenServerCommand::parseSimulatorInput(char* buffer, unordered_map<string, VarInfo*>& symTableUser,
                                            unordered_map<string, VarInfo*>& symTableSimulator) {
  const char* delimiter = ",";
  char* element;
  VarInfo* simVar;
  double newValue;

  //iterating over all vars, in the XML file order, and updating values
  for (int i = 0; i < XML_SIZE; i++) {
    if (i == 0) { element = strtok(buffer, delimiter); }
    else { element = strtok(nullptr, delimiter); }
    simVar = symTableSimulator.at(vars[i]);

    if (element != NULL) {
      newValue = stod(element);
      //we update the value only if it's different from the old value
      if (simVar->getValue() != newValue) {
        simVar->setValue(newValue);

        //if direction is <-, we update the second map
        if (simVar->getDirection() == 0) {
          string secondName = simVar->getSecondName();

          //iterating second map to find the twin
          for (pair<string, VarInfo*> userVar : symTableUser) {
            if (userVar.second->getName() == secondName) {
              userVar.second->setValue(newValue);
              break;
            }
          }
        }
      }
    }
  }
}

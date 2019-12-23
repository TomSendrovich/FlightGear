//
// Created by guy on 13/12/2019.
//

#ifndef MILSTONE1_COMMANDS_OPENSERVERCOMMAND_H_
#include "Command.h"
#define MILSTONE1_COMMANDS_OPENSERVERCOMMAND_H_

class OpenServerCommand : public Command {
 public:
  OpenServerCommand();
  int execute(string* textArr,
              unordered_map<string, Command*>& commandTable,
              unordered_map<string, VarInfo*>& symTableUser,
              unordered_map<string, VarInfo*>& symTableSimulator) override;
  static void openServer(int portNum,
                         unordered_map<string, VarInfo*> symTableUser,
                         unordered_map<string, VarInfo*> symTableSimulator);
  static void runningServer(int client_socket,
                            unordered_map<string, VarInfo*> symTableUser,
                            unordered_map<string, VarInfo*> symTableSimulator);
  static void parseSimulatorInput(char buffer[1024],
                                  unordered_map<string, VarInfo*> symTableUser,
                                  unordered_map<string, VarInfo*> symTableSimulator);
};

#endif //MILSTONE1_COMMANDS_OPENSERVERCOMMAND_H_

//
// Created by guy on 13/12/2019.
//

#ifndef FLIGHTGEAR_COMMANDS_PRINTCOMMAND_H_
#include "Command.h"
#define FLIGHTGEAR_COMMANDS_PRINTCOMMAND_H_

class PrintCommand : public Command {
 public:
  PrintCommand();
  ~PrintCommand() override;
  int execute(string* textArr,
              unordered_map<string, Command*>& commandTable,
              unordered_map<string, VarInfo*>& symTableUser,
              unordered_map<string, VarInfo*>& symTableSimulator,
              queue<const char*>&  commandsToSimulator) override;

};
#endif //FLIGHTGEAR_COMMANDS_PRINTCOMMAND_H_

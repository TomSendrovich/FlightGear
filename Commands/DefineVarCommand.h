//
// Created by guy on 13/12/2019.
//

#ifndef FLIGHTGEAR_COMMANDS_DEFINEVARCOMMAND_H_
#include "Command.h"
#define FLIGHTGEAR_COMMANDS_DEFINEVARCOMMAND_H_

class DefineVarCommand : public Command {
 public:
  DefineVarCommand();
  int execute(string* textArr,
              unordered_map<string, Command*>& commandTable,
              unordered_map<string, VarInfo*>& symTableUser,
              unordered_map<string, VarInfo*>& symTableSimulator,
              queue<const char*>&  commandsToSimulator) override;
};

#endif //FLIGHTGEAR_COMMANDS_DEFINEVARCOMMAND_H_

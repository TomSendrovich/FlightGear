//
// Created by guy on 13/12/2019.
//

#ifndef FLIGHTGEAR_COMMANDS_IFCOMMAND_H_
#include "ConditionParser.h"
#define FLIGHTGEAR_COMMANDS_IFCOMMAND_H_

class IfCommand : public ConditionParser {
 public:
  IfCommand();
  int execute(string* textArr,
              unordered_map<string, Command*>& commandTable,
              unordered_map<string, VarInfo*>& symTableTable,
              unordered_map<string, VarInfo*>& symTableSimulator) override;
};
#endif //FLIGHTGEAR_COMMANDS_IFCOMMAND_H_

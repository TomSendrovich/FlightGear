//
// Created by guy on 13/12/2019.
//

#include "LoopCommand.h"
LoopCommand::LoopCommand() = default;
int LoopCommand::execute() {
  //todo: implement LoopCommand::execute
  updateCondition();
  while (_condition) {
    for (Command* c : _commandsList) {
      c->execute();
    }
    updateCondition();
  }
  return _commandsList.size() + 1;
}

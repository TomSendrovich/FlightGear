cmake_minimum_required(VERSION 3.15)
project(FlightGear)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(FlightGear main.cpp
        Commands/Command.h
        Commands/Command.cpp
        Commands/ConditionParser.h
        Commands/ConditionParser.cpp
        Commands/ConnectCommand.h
        Commands/ConnectCommand.cpp
        Commands/DefineVarCommand.h
        Commands/DefineVarCommand.cpp
        Commands/IfCommand.h
        Commands/IfCommand.cpp
        Commands/LoopCommand.h
        Commands/LoopCommand.cpp
        Commands/PrintCommand.h
        Commands/PrintCommand.cpp
        Commands/SleepCommand.h
        Commands/SleepCommand.cpp
        Commands/AssignCommand.h
        Commands/AssignCommand.cpp
        Commands/OpenServerCommand.h
        Commands/OpenServerCommand.cpp
        VarInfo.cpp
        VarInfo.h
        Data.cpp
        Data.h
        Expressions/Expression.h
        Expressions/Calculator.h
        Expressions/Calculator.cpp
        )

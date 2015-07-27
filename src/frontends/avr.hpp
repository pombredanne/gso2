#ifndef __AVR_HPP__
#define __AVR_HPP__

#include "../frontend.hpp"
// #include "../uops.hpp"
#include "../slots.hpp"
#include <vector>
#include <string>
#include <functional>
#include <string.h>

#include <cppformat/format.h>

#include <iof/fmtr.hpp>
#include <iof/stringizer.hpp>

std::vector<std::function<Instruction *()>> Avr_getAutogeneratedInstructionFactories();

enum AvrRegisterClasses
{
    ALL_REGISTERS,
    REGISTERS_16PLUS,
    REGISTER0,
    REGISTER1,
    REGISTER_EVEN,
    REGISTER_24PLUS_EVEN
};

class AVRRegisterSlot : public RegisterSlot
{
public:
    AVRRegisterSlot(AvrRegisterClasses rclass, bool _write=false, bool _read=true) : RegisterSlot(_write, _read)
    {
        setRegisterClassID(rclass);
        switch(rclass)
        {
        case ALL_REGISTERS:
            setValidArguments({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31});
            break;
        case REGISTERS_16PLUS:
            setValidArguments({16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31});
            break;
        case REGISTER0:
            setValidArguments({0});
            break;
        case REGISTER1:
            setValidArguments({1});
            break;
        case REGISTER_EVEN:
            setValidArguments({0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30});
            break;
        case REGISTER_24PLUS_EVEN:
            setValidArguments({24,26,28,30});
            break;
        default:
            assert(!"Undefined register class");
        }
    }
};

class AvrConstantSlot: public ConstantSlot
{
public:
    AvrConstantSlot(std::vector<std::pair<uint64_t, uint64_t>> ranges_) :
        ConstantSlot(ranges_) {}
};

class AvrMachine : public TargetMachine<uint8_t, 32>
{
public:
    void initialiseRandom()
    {
        for(int i = 0; i < 32; ++i)
        {
            registers[i] = rand() & 0xFF;
        }
    }

    std::string toString()
    {
        std::string s;

        for(int i = 0; i < 32; ++i)
        {
            s += "\t";
            s += "r" + std::to_string(i) + " = " + std::to_string(registers[i]) + "  ";
            if(i % 4 == 3)
                s += "\n";
        }

        return s;
    }

    std::vector<std::function<Instruction* ()>> getInstructionFactories()
    {
        std::vector<std::function<Instruction* ()>> factories;

        factories = Avr_getAutogeneratedInstructionFactories();

        return factories;
    }

    bool operator ==(AvrMachine &rhs)
    {
        for(int i = 0; i < 32; ++i)
        {
            if(registers[i] != rhs.registers[i])
                return false;
        }
        return true;
    }

    typedef AVRRegisterSlot RegisterSlot;
    typedef AvrConstantSlot ConstantSlot;
};


 #include "avr_gen.hpp"



#endif

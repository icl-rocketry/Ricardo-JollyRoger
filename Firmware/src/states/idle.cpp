#include "idle.h"

// Standard imports
#include <memory>

// Third-party imports
#include <libriccore/fsm/state.h>
#include <libriccore/systemstatus/systemstatus.h>
#include <libriccore/commands/commandhandler.h>
#include <libriccore/riccorelogging.h>

// Internal imports
#include "system.h"
#include "config/systemflags_config.h"
#include "config/types.h"

Idle::Idle(System &system,
           Types::CoreTypes::SystemStatus_t &systemstatus,
           Types::CoreTypes::CommandHandler_t &commandhandler)
    : State(SYSTEM_FLAG::STATE_IDLE, systemstatus),
      system(system),
      _commandhandler(commandhandler) {};

void Idle::initialize()
{
    State::initialize(); // call parent initialize first!
    _commandhandler.enableCommands({Commands::ID::Free_Ram});
};

Types::CoreTypes::State_ptr_t Idle::update()
{
    return nullptr;
};

void Idle::exit()
{
    Types::CoreTypes::State_t::exit(); // call parent exit last!
};
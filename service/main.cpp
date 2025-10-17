#include <nodepp/nodepp.h>
#include <ungine/ungine.h>

using namespace nodepp;
using namespace ungine;

global_t global;

#include "../controller/scene/scene_2.cpp"
#include "../controller/scene/scene_0.cpp"
#include "../controller/server/server.cpp"

void onMain() {

    engine::start( 800, 600, "Bros Blaster Arena" );
    engine::set_fps( 60 ); 

    window::set_min_size({ 800, 600 });

    window::mode::set( 
        ungine::window::WINDOW_ALWAYS_RUN |
        ungine::window::WINDOW_RESIZABLE
    );

    game::server::add( "0.0.0.0",8000 );
    auto scene = game::scene::scene_0();

}

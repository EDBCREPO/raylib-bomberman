#include <express/http.h>
#include <nodepp/ws.h>

#pragma once

namespace game { namespace server { void add( string_t host, uint port ) {

    auto app=express::http::add();
    app.USE( express::http::file( "./view" ) );
    
    global.set_attribute( "motion" , vec4_t({ .0, .0, .0, .0 }) );
    auto srv= app.listen( host, port ); ws::server( srv );
    auto mot= global.get_attribute<vec4_t>( "motion" );

    srv.onConnect([=]( ws_t cli ){

        cli.onData([=]( string_t data ){
            auto obj = json::parse( data ); 
        try { 

            mot->x = string::to_float( obj["x"].as<string_t>() );
            mot->y = string::to_float( obj["y"].as<string_t>() );
            mot->z = string::to_float( obj["z"].as<string_t>() );
            mot->w = string::to_float( obj["w"].as<string_t>() );
            
        } catch( except_t ) {} });

        cli.onClose([=](){
            console::log( "disconnect" );
        });

        console::log( "connected" );
    });

}}}
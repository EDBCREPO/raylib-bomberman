#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace game { namespace scene { node_t scene_0() { 
return node::node_scene([=]( ref_t<node_t> self ){

    texture_t init_1   ( "./controller/assets/init_1.png" );
    texture_t init_2   ( "./controller/assets/init_2.png" );
    ptr_t<uchar> alpha ({ 0x00, 0x00, 0x00 });

    self->on2DDraw([=](){ if( alpha[2]!=2 ){ return; }   

        rl::ClearBackground( color_t({ 0, 0, 0, alpha[1] }) ); 
        rl::DrawTexturePro ( init_2.get(), rect_t({
            0, 0, init_2.size().x , init_2.size().y
        }), rect_t({
            window::get_size().x/2- init_2.size().x/6,
            window::get_size().y/2- init_2.size().y/6,
            init_2.size().x/3     , init_2.size().y/3,
        }), vec2_t ({ 0, 0 }), 0, 
            color_t({ 255, 255, 255, alpha[1] })
        );

    });

    self->on2DDraw([=](){ if( alpha[2]!=1 ){ return; }

        rl::ClearBackground( color_t({ 255, 255, 255, alpha[0] }) );
        rl::DrawTexturePro ( init_1.get(), rect_t({
            0, 0, init_1.size().x , init_1.size().y
        }), rect_t({
            window::get_size().x/2- init_1.size().x/6,
            window::get_size().y/2- init_1.size().y/6,
            init_1.size().x/3     , init_1.size().y/3,
        }), vec2_t ({ 0, 0 }), 0, 
            color_t({ 255, 255, 255, alpha[0] })
        );

    });

    self->onLoop([=]( float ){
        if( mouse::is_released( mouse::BUTTON::MOUSE_BUTTON_LEFT ) )
          { scene_2(); self->free(); }
    });

    self->onLoop([=]( float delta ){ ([=](){
        if( !self->exists() ){ return -1; }
    coStart ; 
    
        coDelay( 300 ); alpha[2]++;

        while( alpha[0]< 255 ){
        /*--*/ alpha[0]++; coNext; }

        coDelay( 2000 );

        while( alpha[0]> 0   ){
        /*--*/ alpha[0]--; coNext; }

        coNext; alpha[2]++;

        while( alpha[1]< 255 ){
        /*--*/ alpha[1]++; coNext; }

        coDelay( 2000 );

        while( alpha[1]> 0   ){
        /*--*/ alpha[1]--; coNext; }

        scene_2(); self->free();

    coStop
    })(); });

}); }}}

/*────────────────────────────────────────────────────────────────────────────*/
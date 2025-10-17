#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

void append_recharge_effect( ref_t<node_t> player ) {

    auto pos =*player->get_attribute<transform_2D_t>("transform");
    auto prn = player->get_root(); /*--------------------------*/

    prn->append_child( node_t([=]( ref_t<node_t> self ){

        ptr_t<float> radius = new float( 16 );

        self->onLoop([=]( float delta ){
        if( *radius <= 0 ){ self->free(); }
            *radius -= delta * 80;
        });

        self->on2DDraw([=](){ rl::DrawCircle( 
            pos.translate.position.x + 16, 
            pos.translate.position.y + 16, 
            *radius, rl::WHITE
        ); });

    }) );

}

/*────────────────────────────────────────────────────────────────────────────*/

void append_bomb_expossion( ref_t<node_t> bomb ) {

    function_t<node_t,vec2_t> effect ([=]( vec2_t origin ){ return node::node_2D([=]( ref_t<node_t> self ){

        auto pos = self->get_attribute<transform_2D_t>( "transform" );
        ptr_t<float> radius = new float( 16 ); pos->position = origin;
        
        auto end = type::bind( new bool( false ) );
        timer::timeout([=](){ *end = true; },1000);

        self->set_attribute( "explosion", true );
        self->set_attribute( "collision", rect_t({ 
            origin.x - 13, origin.y - 13, 26, 26
        }));

        self->on2DDraw([=](){

            rl::DrawCircle( 
                pos->translate.position.x, 
                pos->translate.position.y, 
                *radius, rl::WHITE 
            );

            rl::DrawRectangleLines( 
                origin.x - 13, origin.y - 13, 
                26, 26, rl::RED /*---------*/
            );

        });

        self->onLoop([=]( float delta ){
        if( *end ) /*--*/ { 
            if( *radius <= 0 ){ self->free(); }
                *radius -= delta * 100;
        } else {
            *radius = 10 * ( sin(2**timer_)/4+1 );
        } });


    }); });

    auto cll   = bomb->get_root()->get_node( "scene" );
    auto pos   = bomb->get_attribute<transform_2D_t>( "transform" );
    auto nrs   = get_nearest( vec2_t({ 32, 32 }), pos->translate.position );
         nrs.x = floor( nrs.x / 32 ); nrs.y = floor( nrs.y / 32 );

    auto itm   = cll->get_attribute<function_t<void,vec2_t>>( "add_item" );

    ptr_t<node_t*> map_list ( 21*15, nullptr );

    cll->child_iterator([&]( node_t* node ){
    if( !node->has_attribute( "collision" ) ){ return; }

        auto pos = node->get_attribute<rect_t>( "collision" );
        auto nrs = get_nearest( vec2_t({ 32, 32 }), vec2_t({ pos->x, pos->y }) + vec2_t({ 16, 16 }) );
             nrs.x = floor( nrs.x / 32 ); nrs.y = floor( nrs.y / 32 );
             map_list[ type::cast<int>( nrs.y * 21 + nrs.x ) ] = node; 

    }, true );

    cll->append_child( 
        effect( /**/ get_nearest( vec2_t({ 32, 32 }), 
        pos->translate.position + vec2_t({ 16, 16 }) 
    )));

    /* left */
    process::add([=](){ 
    for( auto x= type::cast<int>(nrs.x); x--; ){
         auto y= type::cast<int>(nrs.y); auto w = map_list[y*21+x];
         auto n= get_nearest( vec2_t({ 32, 32 }), vec2_t({ x*32+16, y*32+16 }) );

    if ( w != nullptr ){ 
    if ( w->has_attribute( "item"  ) ){ w->free(); goto DONE; }
    if ( w->has_attribute( "box"   ) ){ w->free(); itm->emit(n);
         cll->append_child( effect(n) ); break; }
    if ( w->has_attribute( "bomb"  ) ){ 
         w->get_attribute<function_t<void>>( "explode" )->emit(); break; }
    if ( w->has_attribute( "explosion" ) ){ break; }
    if ( w->has_attribute( "collision" ) ){ break; }} DONE:;

         cll->append_child( effect( n ) ); SKIP:;
         map_list[ y * 21 + x ] = nullptr;

    } return -1; });

    /* right */
    process::add([=](){ 
    for( auto x= type::cast<int>(nrs.x); x++; ){
         auto y= type::cast<int>(nrs.y); auto w = map_list[y*21+x];
         auto n= get_nearest( vec2_t({ 32, 32 }), vec2_t({ x*32+16, y*32+16 }) );

    if ( w != nullptr ){ 
    if ( w->has_attribute( "item"  ) ){ w->free(); goto DONE; }
    if ( w->has_attribute( "box"   ) ){ w->free(); itm->emit(n);
         cll->append_child( effect(n) ); break; }
    if ( w->has_attribute( "bomb"  ) ){ 
         w->get_attribute<function_t<void>>( "explode" )->emit(); break; }
    if ( w->has_attribute( "explosion" ) ){ break; }
    if ( w->has_attribute( "collision" ) ){ break; }} DONE:;

         cll->append_child( effect( n ) ); SKIP:;
         map_list[ y * 21 + x ] = nullptr;

    } return -1; });

    /* down */
    process::add([=](){ 
    for( auto y= type::cast<int>(nrs.y); y++; ){
         auto x= type::cast<int>(nrs.x); auto w = map_list[y*21+x];
         auto n= get_nearest( vec2_t({ 32, 32 }), vec2_t({ x*32+16, y*32+16 }) );

    if ( w != nullptr ){ 
    if ( w->has_attribute( "item"  ) ){ w->free(); goto DONE; }
    if ( w->has_attribute( "box"   ) ){ w->free(); itm->emit(n);
         cll->append_child( effect(n) ); break; }
    if ( w->has_attribute( "bomb"  ) ){ 
         w->get_attribute<function_t<void>>( "explode" )->emit(); break; }
    if ( w->has_attribute( "explosion" ) ){ break; }
    if ( w->has_attribute( "collision" ) ){ break; }} DONE:;

         cll->append_child( effect( n ) ); SKIP:;
         map_list[ y * 21 + x ] = nullptr;

    } return -1; });

    /* up */
    process::add([=](){ 
    for( auto y= type::cast<int>(nrs.y); y--; ){
         auto x= type::cast<int>(nrs.x); auto w = map_list[y*21+x];
         auto n= get_nearest( vec2_t({ 32, 32 }), vec2_t({ x*32+16, y*32+16 }) );

    if ( w != nullptr ){ 
    if ( w->has_attribute( "item"  ) ){ w->free(); goto DONE; }
    if ( w->has_attribute( "box"   ) ){ w->free(); itm->emit(n);
         cll->append_child( effect(n) ); break; }
    if ( w->has_attribute( "bomb"  ) ){ 
         w->get_attribute<function_t<void>>( "explode" )->emit(); break; }
    if ( w->has_attribute( "explosion" ) ){ break; }
    if ( w->has_attribute( "collision" ) ){ break; }} DONE:;

         cll->append_child( effect( n ) ); SKIP:;
         map_list[ y * 21 + x ] = nullptr;

    } return -1; });
    
}

/*────────────────────────────────────────────────────────────────────────────*/

void shaket_camera( ref_t<node_t> bomb ) {

    auto obj = bomb->get_root()->get_node( "camera" );
    auto pos = obj->get_attribute<transform_2D_t>( "transform" );

    bomb->get_root()->append_child( node_t( [=]( ref_t<node_t> self ){

        auto old_pos = *pos; auto vec = type::bind( vec2_t({0,0}) );
        timer::timeout([=](){ self->free(); }, 300 );

        self->onLoop([=]( float ){
            pos->position = old_pos.position +* vec;
            vec->x = sin( 10 ** timer_ );
            vec->y = cos( 10 ** timer_ );
        });

        self->onClose([=](){ *pos = old_pos; });

    }));
    
}

/*────────────────────────────────────────────────────────────────────────────*/
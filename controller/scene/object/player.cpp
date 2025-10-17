#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

node_t player( vec2_t origin ) { return node::node_2D([=]( ref_t<node_t> self ){

    auto pos = self->get_attribute<transform_2D_t>("transform");
    auto flp = type::bind( vec2_t({ 1, 1 }) );
    auto org = type::bind( vec2_t({ 1, 1 }) );
    auto root= self->get_root();

        pos->scale   *= 30    ; 
        pos->position = origin;

    self->set_attribute( "collision", rect_t({0,0,20,20}) );
    self->set_attribute( "player"   , true );
    
    auto col = self->get_attribute<rect_t>( "collision" );

    function_t<void,vec2_t,ulong> place_bomb([=]( vec2_t pos, ulong timeout ){
        self->get_root()->get_node("scene")
            ->get_attribute<function_t<void,vec2_t,ulong>>("add_bomb")
            ->emit( pos, timeout );
    });

    self->on2DDraw([=](){ if( sprite.null() ){ return; }

        rl::DrawRectangleLines(
            pos->position.x-10, /*-----------*/
            pos->position.y-10, 20, 20, rl::RED 
        );

        rl::DrawEllipse( 
            pos->position.x   , /*------------*/
            pos->position.y+11, 10, 3, rl::BLACK 
        );

        sprite->draw( *pos, rect_t({ 16*org->x, 16*org->y, 16*flp->x, 16*flp->y }) );
    });

    self->onLoop([=]( float delta ){

        col->x = pos->position.x-10;
        col->y = pos->position.y-10,
        pos->rotation = sin(*timer_*3) * delta * 10;

        if  ( key::is_released('E') ){ place_bomb( pos->position, 3000 ); }

        if  ( key::is_down('W') ){ pos->position.y -= 100 * delta; /*------*/ return; }
        elif( key::is_down('S') ){ pos->position.y += 100 * delta; /*------*/ return; }

        if  ( key::is_down('A') ){ pos->position.x -= 100 * delta; flp->x= 1; return; }
        elif( key::is_down('D') ){ pos->position.x += 100 * delta; flp->x=-1; return; }

        pos->rotation = 0;

    });

    self->onCollision([=]( node_t* node ){

        console::log( "collision", *node->get_attribute<string_t>( "name" ) );

        auto col1 = self->get_attribute<rect_t>( "collision" );
        auto col2 = node->get_attribute<rect_t>( "collision" );
            
        if( node->has_attribute( "wall" ) || 
            node->has_attribute( "box"  ) 
        ) {
                
            auto overlap = rl::GetCollisionRec( *col1, *col2 ); 
            if ( overlap.width < overlap.height ){
            if ( col1->x < col2->x) {
                 pos->position.x -= overlap.width;
            } else {
                 pos->position.x += overlap.width;
            }} else {
            if ( col1->y < col2->y ) {
                 pos->position.y -= overlap.height;
            } else {
                 pos->position.y += overlap.height;
            }}

        } elif( node->has_attribute( "item" ) ) {
            append_recharge_effect( self );
            node->free();
        }

    });

});}

/*────────────────────────────────────────────────────────────────────────────*/
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

node_t wall( vec2_t w, vec2_t pos ) { return node::node_2D([=]( ref_t<node_t> self ){

    self->set_attribute( "collision", rect_t({ 32*pos.x+2, 32*pos.y+2, 28, 28 }) );
    self->set_attribute( "wall", true );

    self->on2DDraw([=](){

        sprite->draw( 
            rect_t({ 32*pos.x, 32*pos.y, 32, 32 }), 
            rect_t({ 16*w.x  , 16*w.y  , 16, 16 })
        );

        DrawRectangleLines( 32*pos.x+2, 32*pos.y+2, 28, 28, rl::RED );

    });

});}

/*────────────────────────────────────────────────────────────────────────────*/

node_t box( vec2_t w, vec2_t pos ) { return node::node_2D([=]( ref_t<node_t> self ){

    self->set_attribute( "collision", rect_t({ 32*pos.x+2, 32*pos.y+2, 28, 28 }) );
    self->set_attribute( "box", true ); 

    self->on2DDraw([=](){

        sprite->draw( 
            rect_t({ 32*pos.x, 32*pos.y, 32, 32 }), 
            rect_t({ 16*w.x  , 16*w.y  , 16, 16 })
        );

        DrawRectangleLines( 32*pos.x+2, 32*pos.y+2, 28, 28, rl::RED );

    });

});}

/*────────────────────────────────────────────────────────────────────────────*/

node_t bomb( vec2_t old_origin, ulong timeout ) { return node::node_2D([=]( ref_t<node_t> self ){
    
    vec2_t origin = get_nearest( vec2_t({ 32, 32 }),old_origin);
    auto pos = self->get_attribute<transform_2D_t>("transform");
    auto org = type::bind( vec2_t({ 1, 4 }) );

    self->set_attribute( "collision", rect_t({ origin.x-10, origin.y-10, 20, 20 }) );
    self->set_attribute( "bomb", true );

    self->set_attribute( "explode", function_t<void>([=](){ process::add([=](){ 
        if( !self->exists() ){ return -1; }
        append_bomb_expossion( self ); 
        shaket_camera( self ); self->free();
    return -1; }); }));

    pos->position = origin; 
    pos->scale   *= 20;

    process::add( coroutine::add( COROUTINE(){
        if( !self->exists() ){ return -1; }
    coBegin

        coDelay( timeout ); /*-----------------------------------*/
        self->get_attribute<function_t<void>>( "explode" )->emit();

    coFinish
    }));

    self->on2DDraw([=](){ if( sprite.null() ){ return; }
        rl::DrawRectangleLines( origin.x-10, origin.y-10, 20, 20, rl::RED );
        rl::DrawEllipse( origin.x, origin.y+10, 10, 3, rl::BLACK );
        sprite->draw( *pos, rect_t({ 16*org->x, 16*org->y, 16, 16 }) );
    });

    self->onLoop([=]( float delta ){ 
        pos->scale = vec2_t({ sin(*timer_)/4+1, sin(*timer_)/4+1 }) * 20;
    });

});}

/*────────────────────────────────────────────────────────────────────────────*/

node_t item( vec2_t old_origin, uchar type ) { return node::node_2D([=]( ref_t<node_t> self ){

    vec2_t origin = get_nearest( vec2_t({ 32, 32 }),old_origin);
    auto pos = self->get_attribute<transform_2D_t>("transform");
    auto org = type::bind( vec2_t({ 0, 0 }) );

    self->set_attribute( "collision", rect_t({ origin.x-12, origin.y-12, 25, 25 }) );
    self->set_attribute( "item", true );

    switch( type ){
        case 0x00: org->x=0; org->y=4; break;
        case 0x01: org->x=2; org->y=4; break;
        case 0x02: org->x=3; org->y=4; break;
        case 0x03: org->x=0; org->y=4; break;
        case 0x04: org->x=2; org->y=5; break;
        case 0x05: org->x=3; org->y=5; break;
        case 0x06: org->x=1; org->y=4; break;
        case 0x07: org->x=1; org->y=5; break;
        case 0x08: org->x=1; org->y=6; break;
        default  : /*-------*/ return; break;
    }

    pos->scale *= 25; pos->position = origin;

    self->onLoop([=]( float delta ){ 
        pos->position = origin + vec2_t({ 0., sin(*timer_) });
    });

    self->on2DDraw([=](){ if( sprite.null() ){ return; }
        rl::DrawRectangleLines( origin.x-12, origin.y-12, 25, 25, rl::RED );
        rl::DrawEllipse( origin.x, origin.y+13, 10, 3, rl::BLACK );
        sprite->draw( *pos, rect_t({ 16*org->x, 16*org->y, 16, 16 }) );
    });

});}

void random_item( ref_t<node_t> self, vec2_t origin ) {
    if(( rand() % 5 ) != 0 ){ return; }

    switch( rand() % 7 ){
    case 0 : self->append_child ( item( origin, 0x01 ) ); break;
    case 1 : self->append_child ( item( origin, 0x02 ) ); break;
    case 2 : self->append_child ( item( origin, 0x03 ) ); break;
    case 3 : self->append_child ( item( origin, 0x04 ) ); break;
    case 4 : self->append_child ( item( origin, 0x05 ) ); break;
    case 5 : self->append_child ( item( origin, 0x06 ) ); break;
    case 6 : self->append_child ( item( origin, 0x07 ) ); break;
    default: /*----------------------------------------*/ break; }

}

/*────────────────────────────────────────────────────────────────────────────*/

node_t scene_background() { return node_t([=]( ref_t<node_t> self ){

    self->set_attribute( "add_bomb", function_t<void,vec2_t,ulong>([=]( vec2_t origin, ulong timeout ){
    self->append_child ( bomb( origin, timeout ) ); }));

    self->set_attribute( "add_item", function_t<void,vec2_t>([=]( vec2_t origin ){
    /*-*/ random_item( self, origin ); }));

    auto ref = string_t("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    auto siz = ptr_t<int> ({ 21, 15 });
    auto raw = regex::remove_all( string_t( R"(
        R R R R R R R R R R R R R R R R R R R R R
        R ^ ^ . . . . . . . . . . . . . . . ^ ^ R
        R ^ R . R . R . R . R . R . R . R . R ^ R
        R . . . . . . . . . . . . . . . . . . . R
        R . R . R . R . R . R . R . R . R . R . R
        R . . . . . . . . . . . . . . . . . . . R
        R . R . R . R . R . R . R . R . R . R . R
        R . . . . . . . . . . . . . . . . . . . R
        R . R . R . R . R . R . R . R . R . R . R
        R . . . . . . . . . . . . . . . . . . . R
        R . R . R . R . R . R . R . R . R . R . R
        R . . . . . . . . . . . . . . . . . . . R
        R ^ R . R . R . R . R . R . R . R . R ^ R
        R ^ ^ . . . . . . . . . . . . . . . ^ ^ R
        R R R R R R R R R R R R R R R R R R R R R
    )" ), "\\s+" ); srand( process::now() );

    for( auto i=0; i<raw.size(); i++ ){
        
    if ( raw[i] == '.' ){ raw[i] = rand()%3==0 ? 'O' : '.'; }
         auto idx = ref.find( raw[i] );
    if ( idx.null() ){ continue; }

         float x = ceil( idx[0]    % 4 );
         float y = ceil((idx[0]-x) / 4 );
         float w = ceil( i    % siz[0] );
         float h = ceil((i-w )/ siz[0] );
        
    if ( raw[i] == 'O' )
       { self->append_child( box ( vec2_t({ x, y }), vec2_t({ w, h }) ) ); }
    if ( raw[i] == 'R' )
       { self->append_child( wall( vec2_t({ x, y }), vec2_t({ w, h }) ) ); }
    if ( raw[i] == 'Q' )
       { self->append_child( wall( vec2_t({ x, y }), vec2_t({ w, h }) ) ); }

    }

    self->onLoop  ([=]( float delta ){ *timer_ += delta * 10; });

    self->on2DDraw([=](){

        rl::ClearBackground( rl::GetColor( 0x45283CFF ) );
        rl::DrawRectangleV( 
            vec2_t({ 0, 0 }),
            vec2_t({ 32 * siz[0], 32 * siz[1] }),
            rl::GetColor( 0x847e87FF )
        );

    });

    self->onUIDraw([=](){ rl::DrawFPS( 10, 10 ); });

});}

/*────────────────────────────────────────────────────────────────────────────*/
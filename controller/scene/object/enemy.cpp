#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

node_t enemy( vec2_t old_origin, uchar type ) { return node::node_2D([=]( ref_t<node_t> self ){

    vec2_t origin = get_nearest( vec2_t({ 32, 32 }),old_origin);
    auto pos = self->get_attribute<transform_2D_t>("transform");
    auto org = type::bind( vec2_t({ 0, 0 }) );
    auto ner = type::bind( vec2_t({ 0, 0 }) );

    self->set_attribute( "collision", rect_t({ 0, 0, 0, 0 }) );
    self->set_attribute( "direction", vec2_t({ 0, 0 }) );
    self->set_attribute( "enemy", true );

    auto dir = self->get_attribute<vec2_t>( "direction" );
    auto col = self->get_attribute<rect_t>( "collision" );

    switch( type ){
        case 0x00: org->x=0; org->y=2; break;
        case 0x01: org->x=1; org->y=2; break;
        case 0x02: org->x=2; org->y=2; break;
        case 0x03: org->x=3; org->y=2; break;
        case 0x04: org->x=0; org->y=3; break;
        case 0x05: org->x=1; org->y=3; break;
        case 0x06: org->x=2; org->y=3; break;
        case 0x07: org->x=3; org->y=3; break;
        default  : /*-------*/ return; break;
    }

    pos->scale *= 25; pos->position = origin;

    self->onCollision([=]( node_t* node ){
        if( node->has_attribute( "explosion" ) ){ self->free(); }
    });

    self->onLoop([=]( float delta ){ 
        *ner = get_nearest( vec2_t({ 32, 32 }), pos->translate.position + vec2_t({ 16, 16 }) );
        *col = rect_t({ pos->translate.position.x, pos->translate.position.y, 25, 25 }); 
        if( dir->x != 0 ){ pos->position.y-=( pos->position.y - ner->y ) * delta; }
        if( dir->y != 0 ){ pos->position.x-=( pos->position.x - ner->x ) * delta; }
        pos->position += *dir * 30 * delta; 
    });

    self->on2DDraw([=](){ if( sprite.null() ){ return; }
        rl::DrawEllipse( pos->translate.position.x+12, pos->translate.position.y+23, 10, 3, rl::BLACK );
        sprite->draw( *pos, rect_t({ 16*org->x, 16*org->y, 16, 16 }) );
        rl::DrawCircle( ner->x, ner->y, 5, rl::RED );
        rl::DrawRectangleLines( pos->translate.position.x, pos->translate.position.y, 25, 25, rl::RED );
    });

    self->append_child( node::node_2D([=]( ref_t<node_t> self ){

        auto pos = self->get_attribute<transform_2D_t>( "transform" );
        self->set_attribute( "collision", rect_t({ 0, 0, 0, 0 }) );
        auto col = self->get_attribute<rect_t>( "collision" );
        auto que = type::bind( queue_t<node_t*>() );
        auto vec = ptr_t<bool>({ 0, 0, 0, 0 });
        auto ply = type::bind( new vec2_t({ 0, 0 }) );

        process::add( coroutine::add( COROUTINE(){
            if( !self->exists() ){ return -1; }
        coBegin

            coDelay(5000); ply->x = rand() % 21 * 32;
            /*----------*/ ply->y = rand() % 15 * 32;
            coGoto(0);

        coFinish
        }));

        self->on2DDraw([=](){

            rl::DrawRectangleLines(
                ner->x-32, ner->y-32, 64, 64, rl::RED
            );

            rl::DrawCircle( ner->x-32, ner->y   , 3, rl::RED    );
            rl::DrawCircle( ner->x   , ner->y+32, 3, rl::BLUE   );
            rl::DrawCircle( ner->x+32, ner->y   , 3, rl::YELLOW );
            rl::DrawCircle( ner->x   , ner->y-32, 3, rl::GREEN  );

        });

        self->onCollision([=]( node_t* node ){
        if( node->has_attribute( "explision" ) ||
            node->has_attribute( "box"  )      || 
            node->has_attribute( "wall" )      || 
            node->has_attribute( "bomb" )
        ) { que->push( node ); } 
        if( node->has_attribute( "player" ) ){
            *ply = node->get_attribute<transform_2D_t>( "transform" )
                       ->translate.position;
        } });

        self->onLoop([=]( float ){ *col = rect_t({
            ner->x-32, ner->y-32, 64, 64
        }); 
        
        if( rl::Vector2Distance( *ner, pos->translate.position+vec2_t({ 16,16 }) )<5 
            && !que->empty()
        ) {
        
            auto p1= vec2_t({ ner->x-32, ner->y    }); // left
            auto p2= vec2_t({ ner->x   , ner->y+32 }); // down
            auto p3= vec2_t({ ner->x+32, ner->y    }); // right
            auto p4= vec2_t({ ner->x   , ner->y-32 }); // up

            vec.fill( false );

            auto x=que->first(); while( x!=nullptr ){
            auto r=x->data->get_attribute<rect_t>( "collision" );
            auto y=x->next; 

                if  ( rl::CheckCollisionPointRec( p1, *r ) ){ vec[0] = true; }
                elif( rl::CheckCollisionPointRec( p2, *r ) ){ vec[1] = true; }
                elif( rl::CheckCollisionPointRec( p3, *r ) ){ vec[2] = true; }
                elif( rl::CheckCollisionPointRec( p4, *r ) ){ vec[3] = true; }

            x=y; }

            vec3_t n_dir({ dir->x, dir->y, 10000 }); int c=1; do {

            if( vec[0]==false && (rl::Vector2Distance(p1,*ply)*(dir->x==1?100.:1))<n_dir.z ){ 
                auto dir_= vec3_t({ -1, 0, rl::Vector2Distance(p1,*ply) });
            if( dir_.z < n_dir.z ){ n_dir = dir_; } }

            if( vec[3]==false && (rl::Vector2Distance(p4,*ply)*(dir->y==1?100.:1))<n_dir.z ){
                auto dir_= vec3_t({ 0, -1, rl::Vector2Distance(p4,*ply) });
            if( dir_.z < n_dir.z ){ n_dir = dir_; } }

            if( vec[1]==false && (rl::Vector2Distance(p2,*ply)*(dir->y==-1?100.:1))<n_dir.z ){
                auto dir_= vec3_t({ 0, 1, rl::Vector2Distance(p2,*ply) });
            if( dir_.z < n_dir.z ){ n_dir = dir_; } }

            if( vec[2]==false && (rl::Vector2Distance(p3,*ply)*(dir->x==-1?100.:1))<n_dir.z ){
                auto dir_= vec3_t({ 1, 0, rl::Vector2Distance(p3,*ply) });
            if( dir_.z < n_dir.z ){ n_dir = dir_; } }

            } while(c-->0); dir->x=n_dir.x; dir->y=n_dir.y;

        } que->clear(); });

    }));

});}

/*────────────────────────────────────────────────────────────────────────────*/

node_t enemy_cluster() { return node_t([=]( ref_t<node_t> self ){

    self->append_child( enemy( vec2_t({ 50, 50 }), 0 ) );
/*
    self->append_child( enemy( vec2_t({ 50, 80 }), 1 ) );
    self->append_child( enemy( vec2_t({ 50,110 }), 2 ) );
    self->append_child( enemy( vec2_t({ 50,140 }), 3 ) );
    self->append_child( enemy( vec2_t({ 50,170 }), 4 ) );
    self->append_child( enemy( vec2_t({ 50,200 }), 5 ) );
    self->append_child( enemy( vec2_t({ 50,230 }), 6 ) );
    self->append_child( enemy( vec2_t({ 50,260 }), 7 ) );
*/

});}

/*────────────────────────────────────────────────────────────────────────────*/
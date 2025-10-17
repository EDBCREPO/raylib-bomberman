#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

node_t collision_checker() { return node_t([=]( ref_t<node_t> self ){

    self->onNext([=](){ auto root = self->get_root();
        
        root->child_iterator([&]( node_t* p_node ){

            if( p_node->onCollision.empty() ) /*-*/ { goto DONE; }
            if(!p_node->has_attribute("collision") ){ goto DONE; }
        
        root->child_iterator([&]( node_t* node ){
            if(!node->has_attribute("collision") ){ return; }
            if( node == p_node ) /*------------*/ { return; }

            auto col1 = p_node->get_attribute<rect_t>( "collision" );
            auto col2 =   node->get_attribute<rect_t>( "collision" );

            if( rl::CheckCollisionRecs( *col1, *col2 ) )
              { p_node->onCollision.emit( node ); }

        }, true ); DONE:; 
        }, true );

    });

});}

/*────────────────────────────────────────────────────────────────────────────*/
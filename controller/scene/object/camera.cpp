#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

node_t scene_camera() { return node::camera_2D([=]( ref_t<node_t> self ){

    auto pos = self->get_attribute<transform_2D_t>( "transform" );
    auto cam = self->get_attribute<camera_2D_t>   ( "camera" );

         pos->position = vec2_t({ 32*21/2, 32*15/2 });
         cam->zoom     = 1.1f;

});}

/*────────────────────────────────────────────────────────────────────────────*/
#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include "./object/utils.cpp"
#include "./object/effect.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

#include "./object/background.cpp"
#include "./object/collision.cpp"
#include "./object/player.cpp"
#include "./object/camera.cpp"
#include "./object/enemy.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

namespace game { namespace scene { node_t scene_2() { 
return node::node_scene([=]( ref_t<node_t> self ){

    sprite = type::bind( texture_t( "./controller/assets/sprite.png" ) );

    self->append_child( "scene"    , scene_background() );

    self->append_child( "camera"   , scene_camera () );
    self->append_child( "enemy"    , enemy_cluster() );

    self->append_child( "collision", collision_checker() );
    self->append_child( "player"   , player({ 32+16, 32+16 }) );

    self->onClose([=](){ sprite.free(); });

}); }}}

/*────────────────────────────────────────────────────────────────────────────*/
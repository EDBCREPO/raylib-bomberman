#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

ptr_t<texture_t> sprite;
ptr_t<float>     timer_= new float(0);

/*────────────────────────────────────────────────────────────────────────────*/

float get_nearest( float dst, float src ) { return ceil(src/dst)*dst-dst/2; }

vec2_t get_nearest( vec2_t dst, vec2_t src ) { return vec2_t({
       get_nearest( dst.x, src.x ), get_nearest( dst.y, src.y )
});}

/*────────────────────────────────────────────────────────────────────────────*/
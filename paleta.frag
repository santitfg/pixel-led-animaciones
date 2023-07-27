// Author:
// Title:

#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.141592653589793
#define HALF_PI 1.5707963267948966

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

void main(){

    vec3 fases =vec3(0.4549, 0.0431, 0.0431);
    vec3 off =vec3(0.5);
    vec3 amp =vec3(0.5,1.,.5);
    vec3 frec =vec3(0.7353, 20.6157, 2.);
    frec +=u_time;
    vec2 st=gl_FragCoord.xy/u_resolution.xy;
    st.x*=u_resolution.x/u_resolution.y;
    st*=2.5000;
    float forma =st.x*PI*2.;

    vec3 color=sin(frec+fases+forma)*amp+off;
   color = clamp(color,.0,1.);
    
    gl_FragColor=vec4(color,1.);
}


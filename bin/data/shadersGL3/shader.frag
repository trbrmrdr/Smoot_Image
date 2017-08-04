#version 150
precision highp float;
// this is how we receive the texture
uniform sampler2DRect tex0;

in vec2 texCoordVarying;

out vec4 outputColor;

uniform vec2 u_resolution;
uniform int u_press;
uniform vec2 u_point;
uniform float u_dcol;
uniform float u_mult;
uniform float u_time;
//const float size_box = 10.;
uniform float u_size_box;
//const vec2 boxC = vec2(u_size_box);

#define isPress() (u_press == 1)





vec4 getPointC(in vec2 pos){
#if 1
    vec2 tx = vec2(1.)/u_resolution;
    //vec2 st =  texCoordVarying/u_resolution;
    
    vec2 pos_conv = pos/u_resolution;
    vec4 ret_col = vec4(.0);
    int count = 0;
    for(float i=u_size_box; i>=-u_size_box; --i)
    {
        for(float j=u_size_box; j>=-u_size_box; --j)
        {
            vec2 t_pos = pos_conv + tx * vec2(i,j);
            ret_col = ret_col + texture(tex0, t_pos * u_resolution);
            count++;
            break;
        }
        break;
    }
    return ret_col/count;
#else
    return texture(tex0, pos);
#endif
}

float isBox(in vec2 pos, in vec2 tx,in vec2 st){
    vec2 s2 = tx * u_size_box;
    vec2 dm = step(pos- s2 , st) - step(pos + s2 , st);    
    return 1.-dm.x*dm.y;
}

mat2 rotateMat(in float alpha){
    float c = cos(alpha);
    float s = sin(alpha);
    return mat2(s,-c,c,s);
}

vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0, 
                     0.0, 
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

vec3 hsb2rgb( in float r,in float g,in float b){
    return hsb2rgb(vec3(r,g,b));
}
 
float sin01(in float val){ return .5*(1.+sin(val));} 

float sinab(in float a,in float b ,in float val){ return a + b*(.5 *(1.+ sin(val))); }

void main()
{
    vec4 ret_col = texture(tex0, texCoordVarying);
    //outputColor = ret_col;return;
    vec2 tx = vec2(1.)/u_resolution;
    vec2 st =  texCoordVarying/u_resolution;
    //vec2 st =  (texCoordVarying.xy *2. -  u_resolution)/ min(u_resolution.x,u_resolution.y);
    //st = st*.5+.5;
    
    float is_cur  = isBox(u_point, tx, st);
    if(isPress())
    ret_col = mix( ret_col, vec4(1.,.5,.5,1), 1. - is_cur);
    
    if(
        // && 
        is_cur <= 1.){
        vec4 colC = getPointC(u_point * u_resolution);
        vec4 colT = ret_col;
        
        float colorD = length(colT - colC);
        float hasCol = colorD <=u_dcol ? 1.:0.;
        float distance = length( st - u_point) ;
        st*=10.;
        st+=u_time*.3;
        st*=rotateMat(length(colC) );//+ smoothstep(.0,256.,colorD)*1000.);
        //vec4 newCol = vec4( vec3(hsb2rgb(st.x* sin(distance*.01) + sin(st.y+u_time*3.+colorD) + colorD ,1.,1.)),1.);
        vec4 newCol = vec4( vec3(hsb2rgb(st.x 
        * sqrt(colorD*  ( (u_mult* .0002) * sinab(.2 , .6, u_time*2.)))
        //* sqrt(colorD*  ( u_mult * sinab(.0002 , .0008, .0)))//u_time*3.)) )
        //* sqrt(colorD* u_mult  ) 
        + sin(st.y+u_time*3.),1.,1.)),1.);
    
        ret_col = mix( ret_col , newCol, hasCol);
        //ret_col = newCol;
        //ret_col = mix(ret_col, vec4(1.,0.,0.,0.) ,colorD-1.);
    }
    
    outputColor = ret_col;
    
    
    //outputColor = vec4(
    //                vec3(smoothstep(.0,.3,length(st - u_point) ) )
    //                , 1. );
    //vec2 st = gl_FragCoord.xy;
    //outputColor = vec4(.4,u_point.y, u_point.x,1.);
}
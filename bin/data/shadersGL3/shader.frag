#version 150
precision highp float;
// this is how we receive the texture
uniform sampler2DRect tex0;

in vec2 texCoordVarying;

out vec4 outputColor;

uniform int m_press;
uniform vec2 m_point;
uniform float m_dcol;
uniform float m_time;


#define isPress() (m_press == 1)

const vec2 resolution = vec2(1919, 1080);
const vec2 boxC = vec2(10,10);

vec4 getPointC(in vec2 pos){
        return texture(tex0, pos);
}

float isBox(in vec2 pos, in vec2 tx,in vec2 st){
    vec2 s2 = tx * boxC;
    vec2 dm = step(pos- s2 , st) - step(m_point + s2 , st);    
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
    
void main()
{
    vec4 ret_col = texture(tex0, texCoordVarying);
    
    vec2 tx = vec2(1.)/resolution;
    vec2 st =  texCoordVarying/resolution;
    //vec2 st =  (texCoordVarying.xy *2. -  resolution)/ min(resolution.x,resolution.y);
    //st = st*.5+.5;
    
    float is_cur  = isBox(m_point, tx, st);
    ret_col = mix( ret_col, vec4(1.,.5,.5,1), 1. - is_cur);
    
    if(
        isPress() && 
        is_cur <= 1.){
        vec4 colC = getPointC(m_point * resolution);
        vec4 colT = ret_col;
        
        float colorD = length(colT - colC);
        
        float hasCol = colorD <=m_dcol ? 1.:0.;
        
        float d_speed = 1.-length( st - m_point) ;
        st*=10.;
        st+=m_time*.3;
        st*=rotateMat(length(colC));
        vec4 newCol = vec4( vec3(hsb2rgb(st.x+sin(st.y+m_time*3.) + colorD,1.,1.)),1.);
    
        ret_col = mix( ret_col , newCol, hasCol);
        ret_col = newCol;
        //ret_col = mix(ret_col, vec4(1.,0.,0.,0.) ,colorD-1.);
    }
    
    outputColor = ret_col;
    
    
    //outputColor = vec4(
    //                vec3(smoothstep(.0,.3,length(st - m_point) ) )
    //                , 1. );
    //vec2 st = gl_FragCoord.xy;
    //outputColor = vec4(.4,m_point.y, m_point.x,1.);
}
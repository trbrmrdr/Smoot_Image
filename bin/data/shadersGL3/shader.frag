#version 150

// this is how we receive the texture
uniform sampler2DRect tex0;

in vec2 texCoordVarying;

out vec4 outputColor;
 
uniform float mouseX;
 
uniform vec2 m_point;
 
void main()
{
    outputColor = texture(tex0, texCoordVarying);
    
    vec2 st =  texCoordVarying.xy/vec2(1919,1080);
    outputColor = mix( outputColor, vec4(1.,.5,.5,1.),  1. - smoothstep(.0,.1,length(st - m_point)) );
    
    
    //outputColor = vec4(
    //                vec3(smoothstep(.0,.3,length(st - m_point) ) )
    //                , 1. );
    //vec2 st = gl_FragCoord.xy;
    //outputColor = vec4(.4,m_point.y, m_point.x,1.);
}
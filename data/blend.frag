uniform sampler2D before;  
uniform sampler2D after;  
uniform float blendAlpha;

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

void main(void) {  
    vec2 texcoord = gl_TexCoord[0].xy;
    texcoord.y = texcoord.y;
    texcoord.x = texcoord.x;
    texcoord = texcoord * rotate2d(3.14159265*0.25);
    if(texcoord.x < 1.0) texcoord.x = 0.5+texcoord.x;
    vec4 background = texture2D(after, texcoord);  
    vec4 foreground = texture2D(before, texcoord);  

    gl_FragColor = foreground * blendAlpha + background*(1.0-blendAlpha);
   
}
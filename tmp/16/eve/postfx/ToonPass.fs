#version 120
/*
 *  ToonPass.cpp
 *
 *  Copyright (c) 2013, satcy, http://satcy.net
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
        

uniform SAMPLER2D tex;
uniform float normalEdgeThreshold = 0.2f;
uniform float qLevel = 3.f;
uniform int bSpecular = 1;
uniform vec4 ambient = vec4(0.1f,0.1f,0.1f,1.0f);
uniform vec4 diffuse = vec4(0.9f,0.9f,0.9f,1.0f);
uniform vec4 specular = vec4(1.f,1.f,1.f,1.f);
uniform float shinyness = 100.f;
                             
OUT vec3 v;
OUT vec3 N;

vec3 getNormal(vec2 st){
    vec2 texcoord = clamp(st, 0.001, 0.999);
    return TEXTURE2D(tex, texcoord).rgb;
}

void main(void){
    float dxtex = 1.0 / width;
    float dytex = 1.0 / height;

    vec2 st = GL_TEXCOORD.st;
    // access center pixel and 4 surrounded pixel
    vec3 center = getNormal(st).rgb;
    vec3 left = getNormal(st + vec2(dxtex, 0.0)).rgb;
    vec3 right = getNormal(st + vec2(-dxtex, 0.0)).rgb;
    vec3 up = getNormal(st + vec2(0.0, -dytex)).rgb;
    vec3 down = getNormal(st + vec2(0.0, dytex)).rgb;

    // discrete Laplace operator
    vec3 laplace = abs(-4.0*center + left + right + up + down);
    // if one rgb-component of convolution result is over threshold => edge
    vec4 line = TEXTURE2D(tex, st);
    if(laplace.r > normalEdgeThreshold
    || laplace.g > normalEdgeThreshold
    || laplace.b > normalEdgeThreshold){
        line = vec4(0.0, 0.0, 0.0, 1.0); // => color the pixel green
    } else {
        line = vec4(1.0, 1.0, 1.0, 1.0); // black
    }

    //end Line;

    //start Phong

    //vec3 lightPosition = vec3(100.0, 100.0, 50.0);
    vec3 lightPosition = gl_LightSource[0].position.xyz;

    vec3 L = normalize(lightPosition - v);
    vec3 E = normalize(-v);
    vec3 R = normalize(-reflect(L,N));

    // ambient term
    vec4 Iamb = ambient;

    // diffuse term
    vec4 Idiff = TEXTURE2D( tex, GL_TEXCOORD.st) * diffuse;
    //vec4 Idiff = vec4(1.0, 1.0, 1.0, 1.0) * diffuse;
    Idiff *= max(dot(N,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    // specular term
    vec4 Ispec = specular;
    Ispec *= pow(max(dot(R,E),0.0), shinyness);
    Ispec = clamp(Ispec, 0.0, 1.0); 
    
    vec4 color = Iamb + Idiff;
    if ( bSpecular == 1 ) color += Ispec;
    // store previous alpha value
    float alpha = color.a;
    // quantize process: multiply by factor, round and divde by factor
    color = floor(0.5 + (qLevel * color)) / qLevel;
    // set fragment/pixel color
    color.a = alpha;

    GL_FRAGCOLOR = color * line;
}
                      

/*
 *  LimbDarkeningPass.cpp
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

uniform sampler2D tex0;

uniform vec3 startColor = vec3(1.0f,1.0f,1.0f);
uniform vec3 endColor = vec3(0.0f,0.0f,0.0f);
                             
uniform float radialScale = 1.2f; //0. - 1.0 - 2.0
uniform float brightness = 2.5f;  //0.-1.0, deff:2.5

float fAspect = 1.f; //width / height;

void main() {
    vec2 vUv = GL_TEXCOORD.st;
    vec2 vSunPositionScreenSpace = vec2(0.5);
    
    vec2 diff = vUv - vSunPositionScreenSpace;

    // Correct for aspect ratio

    diff.x *= fAspect;

    float prop = length( diff ) / radialScale;
    prop = clamp( 2.5 * pow( 1.0 - prop, 3.0 ), 0.0, 1.0 );
    
    vec3 color = mix( startColor, endColor, 1.0 - prop );

    vec4 base = texture2D(tex0, vUv);
    GL_FRAGCOLOR = vec4(base.xyz * color, 1.0);
}

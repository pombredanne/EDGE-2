/*
 *  ZoomBlurPass.cpp
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

uniform SAMPLER2D tDiffuse;

uniform float fX = 0.5f; /*centerX*/
uniform float fY = 0.5f; /*centerY*/
uniform float fExposure = 0.48f;
uniform float fDecay = 0.9f;
uniform float fDensity = 0.25f;
uniform float fWeight = 0.25f;
uniform float fClamp = 1.f;

const int iSamples = 20;

void main()
{
    vec2 vUv = GL_TEXCOORD.st;
    vec2 deltaTextCoord = vec2(vUv - vec2(fX,fY));
    deltaTextCoord *= 1.0 /  float(iSamples) * fDensity;
    vec2 coord = vUv;
    float illuminationDecay = 1.0;
    vec4 FragColor0 = vec4(0.0);

    for(int i=0; i < iSamples ; i++)
    {
        coord -= deltaTextCoord;
        vec4 texel = TEXTURE2D(tDiffuse, coord);
        texel *= illuminationDecay * fWeight;

        FragColor0 += texel;

        illuminationDecay *= fDecay;
    }
    FragColor0 *= fExposure;
    FragColor0 = clamp(FragColor0, 0.0, fClamp);
    GL_FRAGCOLOR = FragColor0;
}

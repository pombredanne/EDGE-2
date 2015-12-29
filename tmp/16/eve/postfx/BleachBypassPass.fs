/*
 *  BleachBypassPass.cpp
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
 *  http://code.google.com/p/pyppet/source/browse/pyppet/javascripts/shaders/BleachBypassShader.js?r=a321c4707804bb4b7448c29082f867e18212a9ee&spec=svn3673066e0f453e27af02a82b917b3a426487dfd9
 */

 uniform sampler2D tDiffuse;
 uniform float opacity = 1.f;

 void main() {
     vec2 vUv = GL_TEXCOORD.st;
     vec4 base = texture2D( tDiffuse, vUv );
     
     vec3 lumCoeff = vec3( 0.25, 0.65, 0.1 );
     float lum = dot( lumCoeff, base.rgb );
     vec3 blend = vec3( lum );
     
     float L = min( 1.0, max( 0.0, 10.0 * ( lum - 0.45 ) ) );
     
     vec3 result1 = 2.0 * base.rgb * blend;
     vec3 result2 = 1.0 - 2.0 * ( 1.0 - blend ) * ( 1.0 - base.rgb );
     
     vec3 newColor = mix( result1, result2, L );
     
     float A2 = opacity * base.a;
     vec3 mixRGB = A2 * newColor.rgb;
     mixRGB += ( ( 1.0 - A2 ) * base.rgb );
     
     GL_FRAGCOLOR = vec4( mixRGB, base.a );
 }

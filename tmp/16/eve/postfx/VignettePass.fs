// https://www.shadertoy.com/view/4dS3z3
float iGlobalTime = time;
vec2 iResolution = vec2( width, height );

vec4 ball(vec2 Coords, float size)
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
	vec2 centro = (uv.xy)+Coords.xy; // Calc the Position for the Circel
	
	float isItIn = step(distance(gl_FragCoord.xy, Coords), size); // step return 1.0 if the current uv is in ragen of coord+size
	
	vec4 colorCircel = mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), isItIn); // Return Color with step 0.0 or 1.0
	
	return  colorCircel;
}

float Vignette(vec2 uv)
{
	float OuterVig = 1.5;
	
	float InnerVig = 0.03;
	
	vec2 center = vec2(0.5,0.5);
	float dist  = distance(center,uv );
	float vig = clamp((OuterVig-dist) / (OuterVig-InnerVig),0.0,1.0);
	
	return vig;
}

void main(void)
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
	vec4 color = vec4(1.0,1.0,1.0,1.0);
	
	
	for(float i = 1. ; i <= 5.;i++)
	{
		color *= ball(vec2(sin(iGlobalTime*i)*20.*i+(iResolution.x/2.),cos(iGlobalTime*i)*20.*i+(iResolution.y/2.)), 5.*i);
	}

	
	color *= Vignette(uv);
	GL_FRAGCOLOR = color;
}

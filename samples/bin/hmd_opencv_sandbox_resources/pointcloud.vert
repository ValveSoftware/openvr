#version 430
layout(location = 0) uniform mat4 matrix;
layout(location = 1) uniform mat4 modelview;
layout(location = 2) uniform mat4 modelviewinverse;
layout(location = 3) uniform mat4 projection;
uniform float frameno;
uniform vec4 particleprops; /* ( lifetime divisor 0.05 is good, alpha, upward speed, ? ) */
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 icolor;
out vec4 vColor;
out vec4 direction_forward;
out vec4 worldspace_camera;
out vec4 vposition;
out vec4 viewpert;
out float remaintime;
out float pointsize;
out float frameage;
uniform vec4 antprops; // [antsize, rendermode, max distance, ??? ]
uniform float max_frameno;
const float fadeintime = 0.2;
void main()
{
	vec4 personpos = modelviewinverse * vec4( 0., 0., 0., 1. );
	float distance_to_person = length( position.xyz - personpos.xyz );
	
	frameage = frameno - icolor.a; 
	if( frameage < 0 ) frameage += max_frameno;
	frameage /= distance_to_person + .1; // ** Ali **Far Objects Last Longer

   vposition = position + vec4( 0., pow( frameage*particleprops.b /*CHANGEME*/, 2.0 )*length(icolor.rgb) * 1.0/*CHANGEME*/, 0., 0. );;
	vColor = icolor;
   direction_forward = (vec4( 0.0, 0.0, -1.0, 0.0 ) * modelview); /* Good */
   worldspace_camera = vec4( modelview[3][0], modelview[3][1], modelview[3][2], modelview[3][3] );
	gl_Position = matrix * vposition;
   if( antprops.z > 0.01 && length( gl_Position.z ) > antprops.z ) { gl_PointSize = 0; return; }
   viewpert = gl_Position/gl_Position.w;
   remaintime =  ( (1.0 + fadeintime ) - frameage * particleprops.r /*speed*/);
   if( remaintime > 1.0 ) remaintime = ((1.0+fadeintime) - remaintime) * 5.0;
   
   if( remaintime > 0.0 )
	remaintime = mix( remaintime, 1.0, particleprops.w );
   pointsize = antprops.x*(5.0 + 4.0/gl_Position.w /*CHANGEME*/)*remaintime;
   if( remaintime < 0.0 ) pointsize = 0;
   gl_PointSize = pointsize;
   pointsize = .002 /*Approximation, how should this actually be determined?*/ * pointsize * gl_Position.w;
}


#version 430 core

uniform sampler2D mytexture;
in vec4 vColor;
in vec4 direction_forward;
in vec4 worldspace_camera;
in vec4 vposition;
in float frameage;
in float remaintime;
in float pointsize;
uniform vec4 particleprops; /* ( lifetime divisor 0.05 is good, alpha, upward speed, ? ) */
in vec4 viewpert;
uniform vec4 antprops; // [antsize, rendermode, max distance, ??? ]

layout(location = 0) uniform mat4 matrix;
layout(location = 1) uniform mat4 modelview;
layout(location = 2) uniform mat4 modelviewinverse;
layout(location = 3) uniform mat4 projection;
layout(location = 4) uniform mat4 projectioninverse;

out vec4 outputColor;
void main()
{
	if( antprops.y > 3.5 )
	{
		//vec2 normst = (gl_PointCoord.st - vec2( 0.5 )) * 2.0;
		//float dim = length( normst );
		//if( dim > 1.0 ) discard;

		vec4 Colorx = vec4( pow( vColor.rgb, vec3( 1.0 ) ), 1.0);
		outputColor = vec4( Colorx.xyz, particleprops.g);//vec4( vec3(lighting) * Color.xyz, 1.0 );
		
		vec4 v_clip_coord = ( modelview * vec4( vposition.xyz, 1.0 ) );
		v_clip_coord /= v_clip_coord.w;
		v_clip_coord.z -= remaintime*.1; 
		v_clip_coord = projection * v_clip_coord;
		//vec4 v_clip_coord = matrix * vec4( vposition.xyz, 1.0 );
		float f_ndc_depth = v_clip_coord.z / v_clip_coord.w;
		gl_FragDepth = (1.0 - 0.0) * 0.5 * f_ndc_depth + (1.0 + 0.0) * 0.5;
	}
	else
	{
		vec2 normst = (gl_PointCoord.st - vec2( 0.5 )) * 2.0;
		float dim = length( normst );
		if( dim > 1.0 ) discard;
		vec4 rnormst = vec4( normst, 1.0 - dim, 0.0 );                          /* This is now the vector with z+ = looking right at you */
		vec4 object_to_eye_vector_eye_space  = vec4( matrix * vec4( vposition.xyz, 1.0 ) - matrix * vec4 ( worldspace_camera.xyz, 1.0) );
		object_to_eye_vector_eye_space /= object_to_eye_vector_eye_space.w;
		vec4 surfacenormal = vec4( rnormst.xyz * vec3( 1., -1., 1. ), 0.0 );  /* Worldspace. */
		surfacenormal = modelview * surfacenormal; /*Wrong?*/
		vec4 Color = vec4( vColor.rgb, 0.5);
		vec3 lightpos = normalize( vec3( 0., 1., 0. ) );
		vec3 lightdir = normalize( lightpos );
		float lighting = dot( lightdir, surfacenormal.xyz ) * 0.5 + 0.5;
		outputColor = vec4(vec3(lighting * Color.xyz), particleprops.g);//vec4( vec3(lighting) * Color.xyz, 1.0 );
		vec4 v_clip_coord = ( modelview * vec4( vposition.xyz, 1.0 ) );
		v_clip_coord /= v_clip_coord.w;
		v_clip_coord.z += rnormst.z * pointsize; /* Pull z inward further on center of spheres. */
		v_clip_coord.z /= 1.1;/* Make dots appear closer than other objects */
		v_clip_coord = projection * v_clip_coord;
		//vec4 v_clip_coord = matrix * vec4( vposition.xyz, 1.0 );
		float f_ndc_depth = v_clip_coord.z / v_clip_coord.w;
		gl_FragDepth = (1.0 - 0.0) * 0.5 * f_ndc_depth + (1.0 + 0.0) * 0.5;
	}
}

/*
Don't use this?  Not sure why I have this as a note.

// Append eye_space_pos.z += rnormst.z * pointsize;
float far = gl_DepthRange.far; float near = gl_DepthRange.near;
vec4 eye_space_pos = modelview * vposition;
vec4 clip_space_pos = eye_space_pos * projection;
float ndc_depth = clip_space_pos.z / clip_space_pos.w;
float depth = (((far - near) * ndc_depth) + near + far) / 2.0;
gl_FragDepth = depth;
*/
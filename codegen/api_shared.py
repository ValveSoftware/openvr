import json
import sys
import operator
from functools import reduce

structlist = {}
typedeflist = {}
enumlist = {}

# These are not exported to the json, so we just maintain the small subset here by hand for now.
structparents = {
	'VRTextureWithPose_t' : 'Texture_t',
	'VRTextureWithDepth_t' : 'Texture_t',
	'VRTextureWithPoseAndDepth_t' : 'VRTextureWithPose_t' }

structindices = {}

def loadfile(filename, ns):
	with open(filename) as data_file:
		data = json.load(data_file)

	i = 0
	for struct in data['structs']:
		if(len(struct) > 0 and len(struct['struct'])):
			structname = struct['struct']
			structlist[structname] = 'struct'
			structindices[getclasswithoutnamespace(structname)] = i
		i += 1

	for typedef in data['typedefs']:
		if(len(typedef) > 0 and len(typedef['typedef'])):
			typedeflist[typedef['typedef']] = typedef['type']
	for enum in data['enums']:
		enumname = enum['enumname']
		namespace = getnamespace(enumname)
		if(len(enum) > 0 and len(enum['enumname'])):
			enumlist[enum['enumname'].replace('::','_')] = enum['enumname']
	return data


def paramshavelength(params, paramname):
	checkname = paramname[1:]
	for param in params:
		name = param['paramname']
		name = name[3:len(name)-4]
		sys.stdout.write ('// ' + name + ' ' + checkname + '\n');
		if(name == checkname):
			return True
	return False

def calculateinteropmethodname(classname, methodname):
	return classname.replace('vr::', 'VR::').replace('::', '_') + '_' + methodname

def unmanagedtype(thetype):
	if(thetype == 'float'):
		return 'R4'
	if(thetype == 'double'):
		return 'R8'
	if(thetype == 'char'):
		return 'I1'
	if(thetype == 'byte'):
		return 'U1'
	if(thetype == 'ulong'):
		return 'U8'
	if(thetype == 'uint'):
		return 'U4'
	if(thetype == 'CSteamID'):	# Special case for CSteamID which is a complex C def but is 64-bits.
		return 'U8'
	return 'Struct'

def ctype(thetype):
	thetype = thetype.replace('vr::','')
	if(thetype[0:6] == 'const '):
		thetype = thetype[6:]
	if(thetype[0:6] == 'class '): # for C replaces classes by pointers
		thetype = 'intptr_t'
	if(thetype == 'ulong'):
		return 'unsigned long long'
	if(thetype == 'uint'):
		return 'unsigned long'
	return thetype

def striparraysuffix(thetype):
	if(thetype[len(thetype) - 1] == ']'):
		thetype = thetype[0:thetype.find('[')-1]
	if(thetype == '_Bool'):
		thetype = 'bool'
	return thetype

def converttype(thetype):
	if(thetype[0:11] == 'struct vr::'):
		thetype = thetype[11:]
	if(thetype[0:17] == 'const struct vr::'):
		thetype = thetype[17:]
	if(thetype[0:10] == 'class vr::'):
		thetype = thetype[10:]
	if(thetype == 'const char *const'):
		thetype = 'string'
	#thetype = thetype.replace('::', '_')
	thetypewithnamespace = thetype
	thetype = getclasswithoutnamespace(thetype)
	if(thetype[0:6] == 'const '):
		thetype = thetype[6:]
	if(thetype == 'class CSteamID'):
		thetype = 'ulong'
	if(thetype == 'uint8'):
		thetype = 'byte'
	if(thetype == 'uint8_t'):
		thetype = 'byte'
	if(thetype == 'unsigned short'):
		thetype = 'ushort'
	if(thetype == 'unsigned char'):
		thetype = 'byte'
	#if(thetype == 'unsigned int'):
	#	thetype = 'uint'
	if(thetype == 'uint16'):
		thetype = 'ushort'
	if(thetype == 'uint16_t'):
		thetype = 'ushort'
	if(thetype == 'uint32'):
		thetype = 'uint'
	if(thetype == 'uint32_t'):
		thetype = 'uint'
	if(thetype == 'uint64'):
		thetype = 'ulong'
	if(thetype == 'short'):
		thetype = 'short'
	if(thetype == 'int32'):
		thetype = 'int'
	if(thetype == 'int32_t'):
		thetype = 'int'
	if(thetype == 'int64'):
		thetype = 'long'
	if(thetype == 'uint64_t'):
		thetype = 'ulong'
	if(thetype == '_Bool'):
		thetype = 'bool'
	if(thetype == 'char *'):
		thetype = 'string'
	if(thetype == 'char **'):
		thetype = 'string'
	if(thetype == 'void *'):
		thetype = 'IntPtr'
	if(thetype == 'byte *'):
		thetype = 'IntPtr'
	if(thetype == 'uint8 *'):
		thetype = 'IntPtr'
	if(thetype == 'T *'):
		thetype = 'IntPtr'
	if(thetype == 'func_t'):
		thetype = 'IntPtr'
	if(thetype == 'RenderModel_t *'):
		thetype = 'IntPtr'
	if(thetype == 'RenderModel_TextureMap_t *'):
		thetype = 'IntPtr'
	if(thetype == 'struct VkPhysicalDevice_T *'):
		thetype = 'IntPtr'
	if(thetype == 'struct VkInstance_T *'):
		thetype = 'IntPtr'
	if(thetype == 'SteamLeaderboard_t'):
		thetype = 'ulong'
	if(thetype == 'SteamLeaderboardEntries_t'):
		thetype = 'ulong'
	if(thetype == 'ScreenshotHandle'):
		thetype = 'uint'
	if(thetype == 'AudioPlayback_Status'):
		thetype = 'int'
	if(thetype == 'FriendsGroupID_t'):
		thetype = 'char'
	if(thetype == 'EOverlayToStoreFlag'):
		thetype = 'char'
	if(thetype == 'ESteamAPICallFailure'):
		thetype = 'int'
	if(thetype == 'EGamepadTextInputMode'):
		thetype = 'int'
	if(thetype == 'EGamepadTextInputLineMode'):
		thetype = 'int'
	if(thetype == 'EUniverse'):
		thetype = 'int'
	if(thetype == 'ISteamHTMLSurface::EHTMLMouseButton'):
		thetype = 'int'
	if(thetype == 'ISteamHTMLSurface::EHTMLKeyModifiers'):
		thetype = 'int'
	if(thetype == 'SteamInventoryResult_t'):
		thetype = 'int'
	if(thetype == 'RTime32'):
		thetype = 'ulong'
	if(thetype == 'AccountID_t'):
		thetype = 'uint'
	if(thetype == 'AppId_t'):
		thetype = 'uint'
	if(thetype == 'VRComponentProperties'):
		thetype = 'uint'
	if(thetype == 'SteamAPICall_t'):
		thetype = 'ulong'
	if(thetype == 'UGCQueryHandle_t'):
		thetype = 'ulong'
	if(thetype == 'UGCUpdateHandle_t'):
		thetype = 'ulong'
	if(thetype == 'ClientUnifiedMessageHandle'):
		thetype = 'ulong'
	if(thetype == 'UGCFileWriteStreamHandle_t'):
		thetype = 'ulong'
	if(thetype == 'PublishedFileUpdateHandle_t'):
		thetype = 'ulong'
	if(thetype == 'SteamAPIWarningMessageHook_t'):
		thetype = 'IntPtr'
	if(thetype == 'SteamAPI_PostAPIResultInProcess_t'):
		thetype = 'IntPtr'
	if(thetype == 'SteamAPI_CheckCallbackRegistered_t'):
		thetype = 'IntPtr'
	if(thetype == 'class CGameID'):
		thetype = 'ulong'
	if(thetype == 'PublishedFileId_t'):
		thetype = 'ulong'
	if(thetype == 'UGCHandle_t'):
		thetype = 'ulong'
	if(thetype == 'SNetListenSocket_t'):
		thetype = 'uint'
	if(thetype == 'SNetSocket_t'):
		thetype = 'uint'
	if(thetype == 'TrackedDeviceIndex_t'):
		thetype = 'uint'
	if(thetype == 'VROverlayHandle_t'):
		thetype = 'ulong'
	if(thetype == 'PropertyTypeTag_t'):
		thetype = 'uint'
	if(thetype == 'PropertyContainerHandle_t'):
		thetype = 'ulong'
	if(thetype == 'SpatialAnchorHandle_t'):
		thetype = 'uint'
	if(thetype == 'DriverHandle_t'):
		thetype = 'ulong'
	if(thetype == 'VRActionHandle_t'):
		thetype = 'ulong'
	if(thetype == 'VRActionSetHandle_t'):
		thetype = 'ulong'
	if(thetype == 'VRInputValueHandle_t'):
		thetype = 'ulong'
	if(thetype == 'VRInputOriginHandle_t'):
		thetype = 'ulong'
	if(thetype == 'PathHandle_t'):
		thetype = 'ulong'
	if(thetype == 'VRNotificationId'):
		thetype = 'uint'
	if(thetype == 'TextureID_t'):
		thetype = 'int'
	if(thetype == 'WebConsoleHandle_t'):
		thetype = 'ulong'

	if(thetype[0:7] == 'struct '):
		thetype = thetype[7:]
	if(thetype[0:6] == 'class '):
		thetype = thetype[6:]
	if(thetype[0:6] == 'union '):
		thetype = thetype[6:]
	if(thetype[0] == 'H' and thetype[0:3] != "Hmd" and thetype[0:16] != "HiddenAreaMesh_t"):
		thetype = 'uint'
	if(thetype[0:5] == 'enum '):
		thetype = thetype[5:]
	if(thetype[len(thetype) - 1] == '*' and thetype[len(thetype) - 2] == '*'):
		thetype = 'IntPtr'
	if(thetype[len(thetype) - 1] == '*'):
		if("VRControllerState_t" not in thetype):
			thetype = converttype(thetypewithnamespace[:len(thetypewithnamespace)-2])
		else:
			thetype = converttype(thetype[:len(thetype)-2])
#		thetype = 'IntPtr'
	if(thetype[len(thetype) - 1] == '&'):
		thetype = 'IntPtr'
	if(thetype[len(thetype) - 1] == ']'):
		thetype = thetype[0:thetype.find('[')-1]
	if(thetype in typedeflist):
		thetype = converttype(typedeflist[thetype])
	if(thetypewithnamespace in typedeflist):
		thetype = converttype(typedeflist[thetypewithnamespace])
	return thetype

def getnamespace(classname):
	if(classname.find('::') != -1):
		return classname[:classname.find('::')]
	else:
		return ''

def getclasswithoutnamespace(classname):
	if(classname.rfind('::') != -1):
		return classname[classname.rfind('::')+2:]
	else:
		return classname

def outputenums(namespace, data):
	for enum in data['enums']:
		if(len(enum) > 0 and len(enum['enumname'])):
			ns = getnamespace(enum['enumname'])
			enumname = getclasswithoutnamespace(enum['enumname'])
			if(ns == namespace or (namespace == '' and ns[:1] == 'I')):
				print('public enum '+enumname+'\n{')
				enumNameWithoutE = enumname
				if( enumname.startswith( "E" ) ):
					enumNameWithoutE = enumname[1:]
				for enumvalue in enum['values']:
					entry = enumvalue['name']
					if(entry.startswith(enumname)): entry = entry[len(enumname):] # strip off enum name
					if(entry.startswith(enumNameWithoutE)): entry = entry[len(enumNameWithoutE):] # strip off enum name
					if(entry.startswith('_')): entry = entry[1:] # strip off leading underscore
					print('\t'+entry+' = '+enumvalue['value']+',')
				print('}')

def outputstructfields(struct, data):

	# recursively add base class fields first
	basename = getclasswithoutnamespace(struct['struct'])
	if basename in structparents:
		parentname = structparents[basename]
		i = structindices[parentname];
		outputstructfields(data['structs'][i], data)

	for enumvalue in struct['fields']:
		fieldtype = enumvalue['fieldtype']
		otype = converttype(fieldtype)
		lastchar = fieldtype[len(fieldtype) - 1]
		if(lastchar == ']'):
			#print('\tpublic fixed '+otype+' '+enumvalue['fieldname']+ fieldtype[fieldtype.find('['):]+';')
			dims = map(int, fieldtype[fieldtype.find('[')+1:-1].split(']['))
			size = reduce(operator.mul, dims, 1)
			utype = unmanagedtype(otype)
			if(otype == 'char'):
				#print('\t[MarshalAs(UnmanagedType.ByValTStr, SizeConst = '+str(size)+')]')
				sys.stdout.write('public byte '+enumvalue['fieldname']+'0')
				for i in range(1, size):
					sys.stdout.write(',' + enumvalue['fieldname'] + str(i))
				print(';')
				
				
				print('public string ' + enumvalue['fieldname'] + '\n{')
				print('get\n{')
				print('return new string(new char[] {')
				for i in range(0, size-1):
					print('(char)' + enumvalue['fieldname'] + str(i) + ',')
				print('(char)' + enumvalue['fieldname'] + str(size-1) + '}).TrimEnd(\'\\0\');')
				print('}')
				print('}')
			else:
				#print('\t[MarshalAs(UnmanagedType.ByValArray, SizeConst = '+str(size)+', ArraySubType = UnmanagedType.'+utype+')]')
				for i in range(size):
					print('\tpublic '+otype+' '+enumvalue['fieldname']+str(i)+';'+(' //'+otype+fieldtype[fieldtype.find('['):] if(i==0) else ''))
		else:
			if(otype == 'bool'): print('\t[MarshalAs(UnmanagedType.I1)]')
			if(lastchar == '*'):
				print('\tpublic IntPtr '+enumvalue['fieldname']+';'+' // '+fieldtype)
			else:
				print('\tpublic '+otype+' '+enumvalue['fieldname']+';')

def outputstructs(namespace, data):
	for struct in data['structs']:
		if(len(struct) > 0 and len(struct['struct'])):
			structname = struct['struct']
			#print('//'+structname)
			ns = getnamespace(structname)
			basename = getclasswithoutnamespace(structname)
			if(basename != '(anonymous)' and (ns == namespace or (namespace == '' and ns[:1] == 'I') or (namespace == '' and ns[:1] == 'C'))):
				for key, value in typedeflist.items():
					if(converttype(value) == basename):
						basename = getclasswithoutnamespace(key)
						break
						
				print('[StructLayout(LayoutKind.Sequential)] public struct '+basename+'\n{')
				outputstructfields(struct, data)

				if (basename == 'HmdMatrix34_t'):
					print('#if UNITY_5_3_OR_NEWER')
					print('\n\tpublic Vector3 GetPosition()')
					print('\t{')
					print('\t\treturn new Vector3(m3, m7, -m11);')
					print('\t}')

					print('\n\tpublic bool IsRotationValid()')
					print('\t{')
					print('\t\treturn ((m2 != 0 || m6 != 0 || m10 != 0) && (m1 != 0 || m5 != 0 || m9 != 0));')
					print('\t}')

					print('\n\tpublic Quaternion GetRotation()')
					print('\t{')
					print('\t\tif (IsRotationValid())')
					print('\t\t{')
					print('\t\t\tfloat w = Mathf.Sqrt(Mathf.Max(0, 1 + m0 + m5 + m10)) / 2;')
					print('\t\t\tfloat x = Mathf.Sqrt(Mathf.Max(0, 1 + m0 - m5 - m10)) / 2;')
					print('\t\t\tfloat y = Mathf.Sqrt(Mathf.Max(0, 1 - m0 + m5 - m10)) / 2;')
					print('\t\t\tfloat z = Mathf.Sqrt(Mathf.Max(0, 1 - m0 - m5 + m10)) / 2;')

					print('\n\t\t\t_copysign(ref x, -m9 - -m6);')
					print('\t\t\t_copysign(ref y, -m2 - -m8);')
					print('\t\t\t_copysign(ref z, m4 - m1);')

					print('\n\t\t\treturn new Quaternion(x, y, z, w);')
					print('\t\t}')

					print('\t\treturn Quaternion.identity;')
					print('\t}')

					print('\n\tprivate static void _copysign(ref float sizeval, float signval)')
					print('\t{')
					print('\t\tif (signval > 0 != sizeval > 0)')
					print('\t\t\tsizeval = -sizeval;')
					print('\t}')
					
					print('#endif')
					print('}')
				else:
					print('}')

					# The following structures were originally compiled with pack(4) on Linux & OSX by mistake. Generate
					# a packed version of the structure so we can workaround this in the C# interop code
					if ((basename == 'RenderModel_t') or 
						(basename == 'VRControllerState_t') or 
						(basename == 'RenderModel_TextureMap_t') or
						(basename == 'VREvent_t')):
						print('// This structure is for backwards binary compatibility on Linux and OSX only')
						print('[StructLayout(LayoutKind.Sequential, Pack = 4)] public struct '+basename+'_Packed\n{')
						outputstructfields(struct, data)
						print('\tpublic '+basename+'_Packed('+basename+' unpacked)');
						print('\t{')
						for enumvalue in struct['fields']:
							fieldtype = enumvalue['fieldtype']
							otype = converttype(fieldtype)
							if fieldtype[-1] == ']':
								dims = map(int, fieldtype[fieldtype.find('[')+1:-1].split(']['))
								size = reduce(operator.mul, dims, 1)
								utype = unmanagedtype(otype)
								for i in range(size):
									print('\t\tthis.'+enumvalue['fieldname']+str(i)+' = unpacked.'+enumvalue['fieldname']+str(i)+';')

							else:
								print('\t\tthis.'+enumvalue['fieldname']+' = unpacked.'+enumvalue['fieldname']+';')
						print('\t}')

						print('\tpublic void Unpack(ref '+basename+' unpacked)');
						print('\t{')
						for enumvalue in struct['fields']:
							fieldtype = enumvalue['fieldtype']
							otype = converttype(fieldtype)
							if fieldtype[-1] == ']':
								dims = map(int, fieldtype[fieldtype.find('[')+1:-1].split(']['))
								size = reduce(operator.mul, dims, 1)
								utype = unmanagedtype(otype)
								for i in range(size):
									print('\t\tunpacked.'+enumvalue['fieldname']+str(i)+' = this.'+enumvalue['fieldname']+str(i)+';')

							else:
								print('\t\tunpacked.'+enumvalue['fieldname']+' = this.'+enumvalue['fieldname']+';')
						print('\t}')
						print('}')


def outputinterfaces(namespace, data):
	lastclass = ''
	lastmethod = ''
	for method in data['methods']:
		if (len(method) > 0):
			returntype = converttype(method['returntype'])
			if(method['returntype'][len(method['returntype']) - 1] == '*'):
				# Native methods which return pointers are cast to IntPtr
				returntype = 'IntPtr'

			methodname = method['methodname']
			if(methodname == lastmethod):
				methodname = methodname + repr(count)
				count = count + 1
			else:
				count = 0
			lastmethod = method['methodname']

			classname = method['classname']

			if(namespace != getnamespace(classname)):
				continue

			classname = getclasswithoutnamespace(classname)
			if(classname != lastclass):
				if(lastclass != ''):
					print("\t}\n");

				print("\t[StructLayout(LayoutKind.Sequential)]")
				print("\tpublic struct " + classname + "\n\t{")
				lastclass = classname


			paramlist = []
			if('params' in method):
				for param in method['params']:
					paramtype = converttype(param['paramtype'])
					if(param['paramtype'][len(param['paramtype']) - 1] == '*' and param['paramtype'][len(param['paramtype']) - 2] == '*'):
						paramlist.append('ref ' + paramtype + ' ' + param['paramname'])
					elif(param['paramtype'][len(param['paramtype']) - 1] == '*'):
						if('out_array_count' in param) or ('array_call' in param) or ('array_count' in param) or ('out_array_call' in param):
							paramlist.append('[In, Out] ' + paramtype + '[] ' + param['paramname'])
						elif('out_string_count' in param):
							paramlist.append('System.Text.StringBuilder ' + param['paramname'])
						elif('out_struct' in param):
							paramlist.append('ref '+ paramtype +' ' + param['paramname'])
						elif('out_string' in param):
							paramlist.append('System.Text.StringBuilder ' + param['paramname'])
#						elif(structlist.has_key(paramtype)):
#							paramlist.append(paramtype+' ' + param['paramname'])
#						elif(paramtype[0:1] == 'I' and paramtype != 'IntPtr'):
#							paramlist.append('IntPtr ' + param['paramname'])
						elif (paramtype == 'uint' or paramtype == 'int' or paramtype == 'char' or paramtype == 'bool'):
							# Output params for ints,uints,char
							paramlist.append('ref ' + paramtype + ' ' + param['paramname'])
						elif (paramtype == 'string' and param['paramtype'] == 'const char *'):
							# Managed strings on Windows need to be converted to Utf8 native ptrs
							paramlist.append('IntPtr ' + param['paramname'])
						elif (paramtype == 'string' or paramtype == 'IntPtr'):
							# Strings and IntPtrs are just straight
							paramlist.append(paramtype + ' ' + param['paramname'])
						else:
#							paramlist.append('ref ' + converttype(paramtype) + ' ' + param['paramname'])
							paramlist.append('ref ' + paramtype + ' ' + param['paramname'])
					else:
						paramlist.append(paramtype + ' ' + param['paramname'])

			print("\t\t[UnmanagedFunctionPointer(CallingConvention.StdCall)]")
			print("\t\tinternal delegate "+returntype+" _"+methodname+"("+", ".join(paramlist)+");")
			print("\t\t[MarshalAs(UnmanagedType.FunctionPtr)]")
			print("\t\tinternal _"+methodname+" "+methodname+";\n")
	print("\t}\n\n");


def outputfntables(namespace, data):
	lastclass = ''
	lastmethod = ''
	for method in data['methods']:
		if (len(method) > 0):
			returntype = method['returntype']
			if(returntype == 'class CSteamID'):
				returntype = 'uint64'
			methodname = method['methodname']
			if(methodname == lastmethod):
				methodname = methodname + repr(count)
				count = count + 1
			else:
				count = 0
			lastmethod = method['methodname']

			classname = method['classname']

			if(namespace != getnamespace(classname)):
				continue

			classname = getclasswithoutnamespace(classname)
			if(classname != lastclass):
				if(lastclass != ''):
					print("};\n");

				fntablename = method['classname'].replace('vr::', 'VR::').replace('::', '_') + '_FnTable'
				print("struct " + fntablename + "\n{")
				lastclass = classname

			sys.stdout.write ('\t'+ ctype(returntype) + ' (OPENVR_FNTABLE_CALLTYPE *' + methodname + ')')

			paramlist = []
			if('params' in method):
				for param in method['params']:
					paramlist.append(ctype(param['paramtype']) + ' ' + param['paramname'])

			print('('+", ".join(paramlist)+");")

	print("};\n");

def outputfntabledecls(namespace, data):
	lastclass = ''
	lastmethod = ''
	fntablename = ''
	for method in data['methods']:
		if (len(method) > 0):
			returntype = method['returntype']
			if(returntype == 'class CSteamID'):
				returntype = 'uint64'
			methodname = method['methodname']
			if(methodname == lastmethod):
				methodname = methodname + repr(count)
				count = count + 1
			else:
				count = 0
			lastmethod = method['methodname']

			classname = method['classname']

			if(namespace != getnamespace(classname)):
				continue

			classname = getclasswithoutnamespace(classname)
			if(classname != lastclass):
				if(lastclass != ''):
					print("};\n\n");

				fntablename = method['classname'].replace('vr::', 'VR::').replace('::', '_') + '_FnTable'
				print("static " + fntablename + " g_" + fntablename + " =\n{")
				lastclass = classname

			print('\t&' + fntablename + '_' + methodname + ',')

	print("};\n\n");

def outputfntableinit(namespace, data):
	print("extern void *FindInterface( const char *pchInterfaceName );\n")
	print("void InitializeInterfaceFnTables()\n{")
	lastclass = ''
	for method in data['methods']:
		if (len(method) > 0):
			classname = method['classname']

			if(namespace != getnamespace(classname)):
				continue

			classname = getclasswithoutnamespace(classname)
			if(classname != lastclass):
				instancename = 'g_p' + classname
				print("\t" + instancename + " = ( " + namespace + "::" + classname + " * )FindInterface( " + namespace + "::" + classname + "_Version );")
				lastclass = classname
	print("}\n")

def outputfntableaccess(namespace, data):
	lastclass = ''
	for method in data['methods']:
		if (len(method) > 0):
			classname = method['classname']

			if(namespace != getnamespace(classname)):
				continue

			classname = getclasswithoutnamespace(classname)
			if(classname != lastclass):
				fntablename = method['classname'].replace('vr::', 'VR::').replace('::', '_') + '_FnTable'
				print('FnTableRegistration autoreg_'+fntablename+'( '+classname+'_Version, &g_'+fntablename+' );')
				lastclass = classname

def outputfntablefuncs(namespace, data):
	lastclass = ''
	lastmethod = ''
	instancename = ''
	for method in data['methods']:
		if (len(method) > 0):
			returntype = method['returntype']
			if(returntype == 'class CSteamID'):
				returntype = 'uint64'
			methodname = method['methodname']
			if(methodname == lastmethod):
				methodname = methodname + repr(count)
				count = count + 1
			else:
				count = 0
			lastmethod = method['methodname']

			classname = method['classname']

			if(namespace != getnamespace(classname)):
				continue

			classname = getclasswithoutnamespace(classname)
			if(classname != lastclass):
				if(lastclass != ''):
					print("\n\n");

				instancename = 'g_p' + classname
				print("/** " + classname + " FnTable functions */\n")
				print("static " + namespace + "::" + classname + " *" + instancename + " = nullptr;\n")
				lastclass = classname

			creturntype = ctype(returntype)
			sys.stdout.write ('static '+ creturntype + ' OPENVR_FNTABLE_CALLTYPE '+method['classname'].replace('vr::', 'VR::').replace('::', '_') + '_FnTable_' + methodname)
			paramlist = []
			if('params' in method):
				for param in method['params']:
					paramlist.append(ctype(param['paramtype'])+' '+param['paramname'])

			print('('+", ".join(paramlist)+")\n{")

			paramlist = []
			if('params' in method):
				for param in method['params']:
					paramtype = param['paramtype']
					cparamtype = ctype(paramtype)
					if paramtype != cparamtype:
						if paramtype.startswith('struct'):
							paramlist.append('*('+paramtype+'*)&'+param['paramname'])
						else:
							paramlist.append('('+paramtype+')'+param['paramname'])
					else:
						paramlist.append(param['paramname'])

			# if the method is a destructor
			if(method['methodname'][:8] == 'Destruct'):
				sys.stdout.write ('delete instance;')
			# void returntype does not return
			elif (returntype == 'void'):
				sys.stdout.write (instancename+'->'+method['methodname']+'(')
				if('params' in method):
					sys.stdout.write(", ".join(paramlist))
				sys.stdout.write (');')
			elif (creturntype.startswith('struct') and creturntype != returntype):
				sys.stdout.write (returntype+' result = '+instancename+'->'+method['methodname']+'(')
				if('params' in method):
					sys.stdout.write(", ".join(paramlist))
				sys.stdout.write (');\n')
				sys.stdout.write ('return *('+creturntype+'*)&result;')
			else:
				if creturntype != returntype:
					sys.stdout.write ('return ('+creturntype+')'+instancename+'->'+method['methodname']+'(')
				else:
					sys.stdout.write ('return '+instancename+'->'+method['methodname']+'(')
				if('params' in method):
					sys.stdout.write(", ".join(paramlist))
				sys.stdout.write (');')
			sys.stdout.write ('\n}\n');


###############################
# OUTPUT CLASSES
###############################

def isparamptr(paramname, paramlist):
	for param in paramlist:
		if(param['paramname'] == paramname):
			return param['paramtype'][len(param['paramtype']) - 1] == '*'
	

def outputclasses(namespace, data):

	print("""public class Utils
	{
		public static IntPtr ToUtf8(string managedString)
		{
			if (managedString == null)
			{
				return IntPtr.Zero;
			}

			int size = System.Text.Encoding.UTF8.GetByteCount(managedString) + 1;
			if (buffer.Length < size) buffer = new byte[size];
			int written = System.Text.Encoding.UTF8.GetBytes(managedString, 0, managedString.Length, buffer, 0);
			buffer[written] = 0x00; // null terminate
			IntPtr nativeUtf8 = Marshal.AllocHGlobal(written+1);
			Marshal.Copy(buffer, 0, nativeUtf8, written+1);
			return nativeUtf8;
		}
		private static byte[] buffer = new byte[1024];
	}
	""")

	# the following methods take a mispacked VRControllerState_t on Linux and OSX so we generate
	# a special hacky method to account for that on those platforms.
	controller_packed_methods = ['GetControllerState', 'GetControllerStateWithPose', 'GetComponentState']
	event_packed_methods      = ['PollNextEvent', 'PollNextEventWithPost', 'PollNextOverlayEvent']
	if(namespace == ''):
		namespaceextra = ''
	elif(namespace == 'vr'):
		namespaceextra = 'VR_'
	else:
		namespaceextra = namespace+'_'
	lastclass = ''
	lastmethod = ''
	for method in data['methods']:
		if (len(method) > 0):
			returntype = converttype(method['returntype'])

			interfacename = method['classname']

			if(namespace != getnamespace(interfacename)):
				continue

			interfacename = getclasswithoutnamespace(interfacename)
			methodname = method['methodname']
			if(methodname == lastmethod):
				methodname = methodname + repr(count)
				count = count + 1
			else:
				count = 0
			lastmethod = method['methodname']

			if(interfacename != lastclass):
				if(lastclass != ''):
					print("}\n\n");
				classname = 'C' + interfacename[1:]
				classshort = interfacename[1:]
				print("public class " + classname+"\n{")
				print(interfacename+" FnTable;")
				print("internal " + classname + "(IntPtr pInterface)")
				print("{")
				print("\tFnTable = ("+interfacename+")Marshal.PtrToStructure(pInterface, typeof("+interfacename+"));")
				print("}")

				lastclass = interfacename

			paramlist = []
			# Native params are always ref and include count params
			nativeparamlist = []
			skipparam = ''
			outarray = 0
			outstruct = 0
			twocalls = 0
			skipref = 0
			arraytype = ''
			arrayname = ''
			arrayreplace = ''
			skipcount = ''
			skiptype = ''
			paramtypelist = []
			if('params' in method):
				for param in method['params']:
					paramtype = converttype(param['paramtype'])
					if(param['paramname'] != skipparam):
						if('out_array_count' in param):
							paramlist.append(param['paramname'])
							paramtypelist.append('out ' + paramtype + ' [] ' + param['paramname'])
							nativeparamlist.append(param['paramname'])
							skipparam = param['out_array_count']
							arraytype = paramtype
							arrayname = param['paramname']
							arrayreplace = 'null'
							twocalls = 1
							outarray = 1
							skipref = 1
						elif('out_string_count' in param):
							paramlist.append(param['paramname'])
							paramtypelist.append('out string ' + param['paramname'])
							nativeparamlist.append('pStrBuffer')
							skipparam = param['out_string_count']
							arraytype = paramtype
							arrayname = 'pStrBuffer'
							arrayreplace = 'null'
							twocalls = 1
							skipref = 1
						elif('out_array_call' in param):
							paramlist.append(param['paramname'])
							paramtypelist.append('out ' + paramtype + ' [] ' + param['paramname'])
							nativeparamlist.append(param['paramname'])
							skipparam = param['out_array_call'].split(',')[0]
							arraytype = paramtype
							arrayname = param['paramname']
							arrayreplace = 'null'
							outarray = 1
							skipref = 0
						elif('out_string_count' in param):
							paramlist.append(param['paramname'])
							paramtypelist.append('out ' + paramtype + ' ' + param['paramname'])
							nativeparamlist.append('ref ' + param['paramname'])
							skipparam = param['out_string_count']
						elif('out_struct' in param):
							paramlist.append(param['paramname'])
							paramtypelist.append('out ' + paramtype + ' ' + param['paramname'])
							nativeparamlist.append('ref ' + param['paramname'])
						elif('array_count' in param):
							paramlist.append(param['paramname'])
							paramtypelist.append(paramtype + ' [] ' + param['paramname'])
							nativeparamlist.append(param['paramname'])
							skipparam = param['array_count']
							skipcount = param['paramname']
#						elif(paramtype[0:1] == 'I' and paramtype != 'IntPtr' and not enumlist.has_key(paramtype)):
#							#print("//" + paramtype)
#							paramlist.append(param['paramname'])
#							paramtypelist.append(paramtype + ' ' + param['paramname'])
#							#nativeparamlist.append('ptr')
#							nativeparamlist.append(param['paramname'] + '.GetIntPtr()')
#						elif (structlist.has_key(paramtype)):
#							paramlist.append(' ' + param['paramname'])
#							paramtypelist.append(' ' + paramtype + ' ' + param['paramname'])
#							nativeparamlist.append(' ' + param['paramname'])
						elif (param['paramtype'][len(param['paramtype']) - 1] == '*' and param['paramtype'][len(param['paramtype']) - 2] == '*'):
							paramlist.append('ref ' + param['paramname'])
							paramtypelist.append('ref ' + paramtype + ' ' + param['paramname'])
							nativeparamlist.append('ref ' + param['paramname'])
							if(paramtype != 'uint' and paramtype != 'int' and paramtype != 'char' and paramtype != 'bool'):
								outstruct = 1
								arraytype = paramtype
								arrayname = param['paramname']
								arrayreplace = 'null'
						elif (param['paramtype'][len(param['paramtype']) - 1] == '*' and paramtype != 'string' and paramtype != 'IntPtr'):
							paramlist.append('ref ' + param['paramname'])
							paramtypelist.append('ref ' + paramtype + ' ' + param['paramname'])
							nativeparamlist.append('ref ' + param['paramname'])
							if(paramtype != 'uint' and paramtype != 'int' and paramtype != 'char' and paramtype != 'bool'):
								outstruct = 1
								arraytype = paramtype
								arrayname = param['paramname']
								arrayreplace = 'null'
						elif ('out_string' in param):
							paramlist.append(param['paramname'])
							paramtypelist.append('System.Text.StringBuilder ' + param['paramname'])
							nativeparamlist.append(param['paramname'])
						elif (paramtype == 'string' and param['paramtype'] == 'const char *'):
							paramlist.append(param['paramname'])
							paramtypelist.append(paramtype + ' ' + param['paramname'])
							nativeparamlist.append(param['paramname']+'Utf8')
						else:
							paramlist.append(param['paramname'])
							paramtypelist.append(paramtype + ' ' + param['paramname'])
							nativeparamlist.append(param['paramname'])
					else:
						skiptype = paramtype
						if(skipref):
							nativeparamlist.append('ref ' + param['paramname'])
						elif(skipcount != ''):
							nativeparamlist.append("(" + paramtype +") " + skipcount + ".Length" )
						else:
							nativeparamlist.append(param['paramname'])
						skipparam = 0
			if methodname in controller_packed_methods or methodname in event_packed_methods:
				if methodname in controller_packed_methods:
					packedlist=[w.replace('VRControllerState_t','VRControllerState_t_Packed').replace('string', 'IntPtr') for w in paramtypelist]
				else:
					packedlist=[w.replace('VREvent_t','VREvent_t_Packed') for w in paramtypelist]
				print('// This is a terrible hack to workaround the fact that VRControllerState_t and VREvent_t were ')
				print('// originally mis-compiled with the wrong packing for Linux and OSX.')
				print('[UnmanagedFunctionPointer(CallingConvention.StdCall)]')				
				print('internal delegate '+returntype+' _'+methodname+'Packed('+','.join(packedlist)+');')
				print('[StructLayout(LayoutKind.Explicit)]')
				print('struct '+methodname+'Union\n{')
				print('\t[FieldOffset(0)]\n\tpublic '+interfacename+'._'+methodname+' p'+methodname+';')
				print('\t[FieldOffset(0)]\n\tpublic _'+methodname+'Packed p'+methodname+'Packed;\n}')

			sys.stdout.write ('public '+returntype+' '+methodname+'('+','.join(paramtypelist)+')\n{\n')
	# PROCESS THE STUFF BEFORE THE API CALLS
			if('params' in method):
				for param in method['params']:
					if(skipparam != param['paramname']):
						paramtype = converttype(param['paramtype'])
						if('out_array_count' in param and skipparam != param['out_array_count']):
							skipparam = param['out_array_count']
							sys.stdout.write( '\t'+skiptype+' '+skipparam+' = 0;\n' )
						elif('out_string_count' in param and skipparam != param['out_string_count']):
							skipparam = param['out_string_count']
							#if(isparamptr(skipparam, param)):
							sys.stdout.write( '\t'+skiptype+' '+skipparam+' = 0;\n' )
						elif('out_array_call' in param):
							vals = param['out_array_call'].split(',')
							sys.stdout.write( '\tint '+vals[0]+' = '+vals[1]+' ('+vals[2]+');\n' )
							sys.stdout.write( '\t'+param['paramname']+' = new '+paramtype+'['+vals[0]+'];\n' )
						elif('out_struct' in param):
							sys.stdout.write( '\t'+param['paramname']+' = new '+paramtype+'();\n' )
						elif (param['paramtype'][len(param['paramtype']) - 1] == '*' and (paramtype == 'int' or paramtype == 'uint' or paramtype == 'float' or paramtype == 'long' or paramtype == 'ulong' or paramtype == 'double') and not 'array_count' in param):
							# Out parameters need to be initialized to zero
							sys.stdout.write( '\t'+param['paramname']+' = 0;\n' )
						elif (param['paramtype'][len(param['paramtype']) - 2:] == '**' and (paramtype == 'string')):
							# Out parameters need to be initialized to zero
							sys.stdout.write( '\t'+param['paramname']+' = "";\n' )
						elif (param['paramtype'][len(param['paramtype']) - 1] == '*' and (paramtype == 'char')):
							# Out parameters need to be initialized to zero
							sys.stdout.write( '\t'+param['paramname']+' = (char) 0;\n' )
						elif (param['paramtype'][len(param['paramtype']) - 1] == '*' and (paramtype == 'bool')):
							# Out parameters need to be initialized to zero
							sys.stdout.write( '\t'+param['paramname']+' = false;\n' )
						elif (paramtype == 'string' and param['paramtype'] == 'const char *'):
							# In strings need to be converted to UTF-8
							sys.stdout.write( '\tIntPtr '+param['paramname']+'Utf8 = Utils.ToUtf8('+param['paramname']+');\n')
						#elif(paramtype[0:1] == 'I' and paramtype != 'IntPtr' and not enumlist.has_key(paramtype)):
							# Returning an interface ptr needs ptr defined
							# sys.stdout.write( '\tIntPtr ptr = IntPtr.Zero;\n' )
					

	# PROCESS THE FIRST OPTIONAL API CALL
			if(twocalls):
				paramsnull = ','.join(map((lambda x: (arrayreplace if (x==arrayname) else x)),nativeparamlist))
				sys.stdout.write ('\t'+('' if (returntype=='void') else (returntype+' result = '))+'FnTable.'+methodname+'('+paramsnull+');\n')

	# PROCESS IN BETWEEN THE API CALLS
	# Allocate the array or buffer
			if('params' in method):
				for param in method['params']:
					if('out_array_count' in param):
						sys.stdout.write( '\t'+param['paramname']+'= new ' + converttype(param['paramtype']) + '['+param['out_array_count']+'];\n' )
					if('out_string_count' in param):
						sys.stdout.write( '\tSystem.Text.StringBuilder pStrBuffer = new System.Text.StringBuilder((int)'+skipparam+');\n' )


	# PROCESS THE FINAL API CALL
			if methodname in controller_packed_methods:
				packedparamlist = [w.replace('pControllerState','state_packed')
								    .replace('unControllerStateSize','(uint)System.Runtime.InteropServices.Marshal.SizeOf(typeof(VRControllerState_t_Packed))')
									for w in nativeparamlist]

				print('#if !UNITY_METRO')
				print('\tif ((System.Environment.OSVersion.Platform == System.PlatformID.MacOSX) ||')
				print('\t    (System.Environment.OSVersion.Platform == System.PlatformID.Unix))')
				print('\t{')
				print('\t\t'+methodname+'Union u;')
				print('\t\tVRControllerState_t_Packed state_packed = new VRControllerState_t_Packed(pControllerState);')
				print('\t\tu.p'+methodname+'Packed = null;')
				print('\t\tu.p'+methodname+' = FnTable.'+methodname+';')
				print('\t\t'+returntype+' packed_result = u.p'+methodname+'Packed('+','.join(packedparamlist)+');\n')
				print('\t\tstate_packed.Unpack(ref pControllerState);')
				print('\t\treturn packed_result;')
				print('\t}')
				print('#endif')
			if methodname in event_packed_methods:
				packedparamlist = [w.replace('pEvent','event_packed')
									.replace('uncbVREvent','(uint)System.Runtime.InteropServices.Marshal.SizeOf(typeof(VREvent_t_Packed))')
									for w in nativeparamlist]

				print('#if !UNITY_METRO')
				print('\tif ((System.Environment.OSVersion.Platform == System.PlatformID.MacOSX) ||')
				print('\t    (System.Environment.OSVersion.Platform == System.PlatformID.Unix))')
				print('\t{')
				print('\t\t'+methodname+'Union u;')
				print('\t\tVREvent_t_Packed event_packed = new VREvent_t_Packed();')
				print('\t\tu.p'+methodname+'Packed = null;')
				print('\t\tu.p'+methodname+' = FnTable.'+methodname+';')
				print('\t\t'+returntype+' packed_result = u.p'+methodname+'Packed('+','.join(packedparamlist)+');\n')
				print('\t\tevent_packed.Unpack(ref pEvent);')
				print('\t\treturn packed_result;')
				print('\t}')
				print('#endif')


			savereturn = returntype
			if(method['returntype'][len(method['returntype']) - 1] == '*'):
				returntype = 'IntPtr'
			if(returntype == 'void'):
				if(len(nativeparamlist) > 0):
					sys.stdout.write ('\tFnTable.'+methodname+'('+','.join(nativeparamlist)+');\n')
				else:
					sys.stdout.write ('\tFnTable.'+methodname+'();\n')
			else:
				if(len(nativeparamlist) > 0):
					#if(returntype[0:1] == 'I' and returntype != 'IntPtr'):
					#	sys.stdout.write ('\tC'+returntype[1:]+' result = new C'+returntype[1:]+'(FnTable.'+methodname+'('+','.join(nativeparamlist)+'));\n')
					if(returntype == 'string'):
						sys.stdout.write ('\tstring result = Marshal.PtrToStringAuto(FnTable.'+methodname+'('+','.join(nativeparamlist)+'));\n')
					else:
						sys.stdout.write ('\t'+('' if twocalls else returntype)+' result = FnTable.'+methodname+'('+','.join(nativeparamlist)+');\n')
				else:
					if(returntype == 'string'):
						sys.stdout.write ('\tstring result = Marshal.PtrToStringAuto(FnTable.'+methodname+'());\n')
					else:
						sys.stdout.write ('\t'+('' if twocalls else returntype)+' result = FnTable.'+methodname+'();\n')


	# PROCESS AFTER THE API CALL
			if('params' in method):
				for param in method['params']:
					paramtype = converttype(param['paramtype'])
					if('out_string_count' in param):
						sys.stdout.write( '\t'+param['paramname']+'  = pStrBuffer.ToString();\n' )
					elif (paramtype == 'string' and param['paramtype'] == 'const char *'):
						sys.stdout.write( '\tMarshal.FreeHGlobal('+param['paramname']+'Utf8);\n')
					#elif(paramtype[0:1] == 'I' and paramtype != 'IntPtr' and not enumlist.has_key(paramtype)):
						# Interface. Need to construct a class wrapper
						#sys.stdout.write( '\t'+param['paramname']+'= new C' + paramtype[1:] + '(ptr);\n' )
		
			if(method['returntype'][len(method['returntype']) - 1] == '*'):
				if(savereturn == 'string'):
					sys.stdout.write ('\treturn Marshal.PtrToStringAnsi(result);\n')
				else:
					sys.stdout.write ('\treturn ('+savereturn+') Marshal.PtrToStructure(result, typeof('+savereturn+'));\n')
			elif(returntype != 'void'):
				sys.stdout.write ('\treturn result;\n')
			sys.stdout.write ('}\n')

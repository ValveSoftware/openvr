Codegen
---
The OpenVR SDK ships with metadata for its headers at [openvr_api.json](https://github.com/ValveSoftware/openvr/blob/master/headers/openvr_api.json).

This can be used to generate bindings for other languages such as C or C#.

We include the scripts we use to generate the existing bindings that ship as part of the SDK here.

### Usage

These scripts assume Python 2.7.

We use [Artistic Style](http://astyle.sourceforge.net/) for formatting.

#### C# bindings:

```
python openvr_interop.cs.py > openvr_interop.cs
astyle -T -O openvr_interop.cs
```

#### C bindings:

```
python openvr_capi.h.py > openvr_capi.h
astyle -T -O openvr_capi.h
```

#### C implementation:

This file is build into openvr_api.dll and is used by both the C and C# bindings.

```
python openvr_capi.cpp.py > openvr_capi.cpp
astyle -T -O openvr_capi.cpp
```
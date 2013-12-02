#include "AS3/AS3.h"
#include "decode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
	

void WebP_getImageSize() __attribute__((used,
  annotate("as3sig:public function WebP_getImageSize(data:ByteArray):String"),
  annotate("as3package:info.smoche.libWebp.Decode"),
  annotate("as3import:flash.utils.ByteArray")
));

void WebP_getImageSize()
{
  // AS3 => C
  inline_as3("\
    var as3_data:int = CModule.malloc(data.length);   \
    CModule.writeBytes(as3_data, data.length, data);  \
    var as3_dataLength:int = data.length;             \
  ");
  uint8_t* src = NULL;
  size_t src_length = 0;
  AS3_GetScalarFromVar(src, as3_data);
  AS3_GetScalarFromVar(src_length, as3_dataLength);

  // get width and height.
  int width = 0;
  int height = 0;
  int ret = WebPGetInfo(src, src_length, &width, &height);
  inline_as3("CModule.free(as3_data);\n");

  // build result
  AS3_DeclareVar(ret2, String);
  try {
    if (ret != 0) {
      char buf[32];
      int len = snprintf(buf, sizeof(buf), "%d,%d", width, height);
      AS3_CopyCStringToVar(ret2, buf, len);
    } else {
      AS3_CopyCStringToVar(ret2, "-1,-1", 5);
    }
  } catch (...) {
    AS3_CopyCStringToVar(ret2, "-2,-2", 5);
  }

  AS3_ReturnAS3Var(ret2);
}



void WebP_decode() __attribute__((used,
  annotate("as3sig:public function WebP_decode(data:ByteArray):BitmapData"),
  annotate("as3package:info.smoche.libWebp.Decode"),
  annotate("as3import:flash.display.BitmapData"),
  annotate("as3import:flash.geom.Rectangle"),
  annotate("as3import:flash.utils.ByteArray")
));

void WebP_decode()
{
  // AS3 => C
  inline_as3("\
    var as3_data:int = CModule.malloc(data.length);   \
    CModule.writeBytes(as3_data, data.length, data);  \
    var as3_dataLength:int = data.length;             \
  ");
  uint8_t* src = NULL;
  size_t src_length = 0;
  AS3_GetScalarFromVar(src, as3_data);
  AS3_GetScalarFromVar(src_length, as3_dataLength);

  // decode
  int width = 0;
  int height = 0;
  uint8_t* dst = WebPDecodeARGB(src, src_length, &width, &height);
  inline_as3("CModule.free(as3_data);\n");

  // build result
  AS3_DeclareVar(result, BitmapData);

  if (dst) {
    AS3_DeclareVar(as3_dst, int);     // C::dst => AS3
    AS3_CopyScalarToVar(as3_dst, dst);

    AS3_DeclareVar(as3_width, int);   // C::width => AS3
    AS3_CopyScalarToVar(as3_width, width);

    AS3_DeclareVar(as3_height, int);  // C::height => AS3
    AS3_CopyScalarToVar(as3_height, height);

    inline_as3("\
      var ba:ByteArray = new ByteArray();                     \
      CModule.readBytes(as3_dst, as3_width*as3_height*4, ba); \
      ba.position = 0;                                        \
      result = new BitmapData(as3_width, as3_height, true);   \
      var rect:Rectangle = new Rectangle(0, 0, as3_width-1, as3_height-1); \
      result.setPixels(rect, ba); \
    ");
    free(dst);
  }
  AS3_ReturnAS3Var(result);
}


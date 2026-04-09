#ifndef TIM2_H
#define TIM2_H

#include "types.h"

typedef struct {
    char FileId[4];       // ファイルID
    u_char FormatVersion; // ファイルフォーマットバージョン
    u_char FormatId;      // フォーマットID
    u_short Pictures;     // ピクチャデータ個数
    char Reserved[8];     // 16バイトアラインメント
} TIM2_FILEHEADER;

typedef struct {
    u_int TotalSize;       // ピクチャデータ全体のバイトサイズ
    u_int ClutSize;        // CLUTデータ部のバイトサイズ
    u_int ImageSize;       // イメージデータ部のバイトサイズ
    u_short HeaderSize;    // 画像ヘッダサイズ
    u_short ClutColors;    // クラットサイズ（クラット部の色数）
    u_char PictFormat;     // 画像ID
    u_char MipMapTextures; // MIPMAP枚数
    u_char ClutType;       // クラット部種別
    u_char ImageType;      // イメージ部種別
    u_short ImageWidth;    // 画像横サイズ(ビット数ではありません)
    u_short ImageHeight;   // 画像縦サイズ(ビット数ではありません)

    u_int GsTex0[2]; // TEX0
    u_int GsTex1[2]; // TEX1
    u_int GsRegs;    // TEXA, FBA, PABEの混合ビット
    u_int GsTexClut; // TEXCLUT(下位32bitそのまま)
} TIM2_PICTUREHEADER;

#endif

/* -------------------------------------------------------------------------- *
 * メディアとデザイン「数値化と変換」
 * アスキーアート製造プログラム
 * -------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *fin;
  BITMAPFILEHEADER bmpfh;
  BITMAPINFOHEADER bmpih;
  BYTE *lineBuf;
  size_t pos, lineSize;
  const char ptn[] = "@#*|!:,. "; /* アスキーアートに使う文字 */
  int bwlv;
  int x, y, idx;

  if (argc < 2) {
    fprintf(stderr, "ファイルを指定してください。\n");
    return 0;
  }

  if ((fin = fopen(argv[1], "rb")) == NULL) {
    fprintf(stderr, "ファイル「%s」を開けませんでした。\n", argv[1]);
    return 0;
  }

  if (fread(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fin) != 1) {
    fprintf(stderr, "ファイルヘッダが正しく読み込めませんでした。\n");
    fclose(fin);
    return 0;
  }

  if (*(BYTE *)&bmpfh.bfType != 'B' || *((BYTE *)&bmpfh.bfType + 1) != 'M') {
    fprintf(stderr, "ファイル「%s」はビットマップ画像ではありません。\n", argv[1]);
    fclose(fin);
    return 0;
  }

  if (fread(&bmpih, sizeof(BITMAPINFOHEADER), 1, fin) != 1) {
    fprintf(stderr, "ファイルヘッダが正しく読み込めませんでした。\n");
    fclose(fin);
    return 0;
  }

  if (bmpih.biSize != 40 || bmpih.biBitCount != 24 || bmpih.biCompression != 0) {
    fprintf(stderr, "このプログラムは 24 ビット無圧縮 Windows ビットマップのみに対応しています。\n");
    fclose(fin);
    return 0;
  }

  lineSize = sizeof(BYTE) * bmpih.biWidth * 3;
  
  if ((lineBuf = (BYTE *)malloc(lineSize)) == NULL) {
    fprintf(stderr, "作業用メモリを確保できませんでした。\n");
    fclose(fin);
    return 0;
  }

  pos = bmpfh.bfOffBits + (bmpih.biHeight - 1) * lineSize;
  for (y = bmpih.biHeight - 1; y >= 0; y--) {
    fseek(fin, pos, SEEK_SET);
    fread(lineBuf, 1, bmpih.biWidth * 3, fin);
    for (x = 0, idx = 0; x < bmpih.biWidth; x++, idx += 3) {
      bwlv = (int)(lineBuf[idx] * 0.114 + lineBuf[idx + 1] * 0.587 + lineBuf[idx + 2] * 0.299);
      putchar(ptn[(sizeof ptn / sizeof ptn[0] - 1) * bwlv / 256]);
      putchar(' ');
    }
    pos -= lineSize;
    putchar('\n');
  }

  free(lineBuf);
  fclose(fin);
  return 0;
}

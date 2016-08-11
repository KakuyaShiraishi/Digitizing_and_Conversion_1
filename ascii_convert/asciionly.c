/* -------------------------------------------------------------------------- *
 * ���f�B�A�ƃf�U�C���u���l���ƕϊ��v
 * �A�X�L�[�A�[�g�����v���O����
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
  const char ptn[] = "@#*|!:,. "; /* �A�X�L�[�A�[�g�Ɏg������ */
  int bwlv;
  int x, y, idx;

  if (argc < 2) {
    fprintf(stderr, "�t�@�C�����w�肵�Ă��������B\n");
    return 0;
  }

  if ((fin = fopen(argv[1], "rb")) == NULL) {
    fprintf(stderr, "�t�@�C���u%s�v���J���܂���ł����B\n", argv[1]);
    return 0;
  }

  if (fread(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fin) != 1) {
    fprintf(stderr, "�t�@�C���w�b�_���������ǂݍ��߂܂���ł����B\n");
    fclose(fin);
    return 0;
  }

  if (*(BYTE *)&bmpfh.bfType != 'B' || *((BYTE *)&bmpfh.bfType + 1) != 'M') {
    fprintf(stderr, "�t�@�C���u%s�v�̓r�b�g�}�b�v�摜�ł͂���܂���B\n", argv[1]);
    fclose(fin);
    return 0;
  }

  if (fread(&bmpih, sizeof(BITMAPINFOHEADER), 1, fin) != 1) {
    fprintf(stderr, "�t�@�C���w�b�_���������ǂݍ��߂܂���ł����B\n");
    fclose(fin);
    return 0;
  }

  if (bmpih.biSize != 40 || bmpih.biBitCount != 24 || bmpih.biCompression != 0) {
    fprintf(stderr, "���̃v���O������ 24 �r�b�g�����k Windows �r�b�g�}�b�v�݂̂ɑΉ����Ă��܂��B\n");
    fclose(fin);
    return 0;
  }

  lineSize = sizeof(BYTE) * bmpih.biWidth * 3;
  
  if ((lineBuf = (BYTE *)malloc(lineSize)) == NULL) {
    fprintf(stderr, "��Ɨp���������m�ۂł��܂���ł����B\n");
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

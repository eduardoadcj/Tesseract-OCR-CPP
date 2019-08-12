#include <tesseract/baseapi.h>
#include <tesseract/ocrclass.h>
#include <leptonica/allheaders.h>
#include <thread>
#include <fstream>

#DEFINE LINE_SIZE 100

using namespace std;

FILE *file;

void monitorProgress(ETEXT_DESC *monitor, int page);
void ocrProcess(tesseract::TessBaseAPI *api, ETEXT_DESC *monitor);

void monitorProgress(ETEXT_DESC *monitor, int page) {
  while (1) {
      printf( "\r%3d%%", monitor[page].progress);
      fflush (stdout);
      if (monitor[page].progress==100)
          break;
  }
}

void ocrProcess(tesseract::TessBaseAPI *api, ETEXT_DESC *monitor) {
  api->Recognize(monitor);
}

int open_file(char *path)
{
  file = fopen(path, "rt+");
  if(file == NULL){

    file = fopen(path, "wt");
    if(file == NULL)
      return 0;

    fclose(file);
    file = fopen(path, "rt+");

  }
  return 1;
}

int write_file(char *text)
{

  char line[LINE_SIZE];
  //pegar o tamanho da entrada e dividir no vetor de linhas.
  //mandar escrever ate terminar as linhas do arquivo.

}

int main(int argc, char **argv) {

  if(argc != 4){
    printf("Modo de usar: %s <imagem de entrada> <arquivo de saida>\n", argv[0]);
  }

  char *input_img_path, *output_file_path, *outText;

  input_img_path = argv[1];
  output_file_path = argv[2];

  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  ETEXT_DESC *monitor = new ETEXT_DESC();

  if (api->Init(NULL, "por")) {
    printf("Falha ao iniciar o tesseract.\n");
    return 1;
  }

  api->SetPageSegMode(tesseract::PSM_AUTO);
  Pix *image = pixRead(input_img_path);
  if (!image) {
    printf("Imagem não encontrada!\n");
    return 2;
  }
  api->SetImage(image);

  if(!open_file(output_file_path)){
    printf("Falha ao criar o arquivo de saída!\n");
    return 2;
  }

  int page = 0;
  std::thread t1(ocrProcess, api, monitor);
  std::thread t2(monitorProgress, monitor, page);
  t1.join();
  t2.join();

  pixDestroy(&image);
  *outText = api->GetUTF8Text();

  //mandar escrever o arquivo agora

  if (outText)
    delete [] outText;

  api->End();

  fclose(file);

  return 0;

}

// to compile
// g++ -o ex reader_monitor.cpp -llept -ltesseract -lpthread

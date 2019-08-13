#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main()
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    /*
      Inicializando a API com a lingua portuguesa
      O segundo primeiro parametro diz respeito ao caminho da pasta tessdata.
      Como ela se encontra em um lugar padronizado, não tem a necessidade de
      especificar o caminho, mas caso ele não encontre a pasta ou por algum
      outro motivo não consiga inicializar, a condição do if é satisfeita
      então o programa imprime uma mensagem de erro e finaliza.
    */
    if (api->Init(NULL, "por")) {
        printf("Não foi possivel iniciar o tesseract.\n");
        exit(1);
    }

    /*
      O Pix é uma struct pertencente a biblioteca Leptonica que é uma
      dependência do tesseract. A utilização dela facilita a manipulação
      de imagens.
      As linhas a seguir servem para carregar a imagem em memória.
    */
    Pix *image = pixRead("image.png");
    api->SetImage(image);

    // Obtendo o resultado a partir da imagem definida
    outText = api->GetUTF8Text();
    printf("%s",outText);

    // Destruindo objeto utilizado e liberando memória
    api->End();
    delete [] outText;
    pixDestroy(&image);

    return 0;
}

// g++ -o ex reader.cpp -llept -ltesseract

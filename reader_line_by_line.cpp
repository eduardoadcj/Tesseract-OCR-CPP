#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main()
{

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "por")) {
        printf("Não foi possivel iniciar o tesseract.\n");
        exit(1);
    }

    Pix *image = pixRead("image.png");
    api->SetImage(image);

    /*
      A linha de codigo a seguir utiliza uma função que identifica componentes
      dentro de uma imagem e os retorna em uma struct do tipo Boxa.
      No caso a seguir, a função obtem todas as linhas identificando
      somente o texto.
      O primeiro parametro definido indica o tipo de componente, no caso linhas
      do texto.
      O segundo parametro indica que apenas estruturas de texto devem ser analisadas.
      Os parametros restantes diz respeito a informacões adicionais que serão passados para a struct Boxa.
      Como no caso a ideia é apenas a identificação das linhas, não é necessário.
    */
    Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);

    printf("%d linhas foram identificadas.\n", boxes->n);
    for (int i = 0; i < boxes->n; i++) {

      /*
        Obtem um componente específico de uma struct Boxa no tipo BOX,
        a partir de um numero iterador.
      */
      BOX* box = boxaGetBox(boxes, i, L_CLONE);

      /*
        A linha a seguir serve para definir um ponto específico da imagem
        em que a api de reconhecimento deve trabalhar. Esta cria um sub
        retangulo dentro da imagem original. Desta forma, ela recebe como
        parametro, a posição x e y desse retangulo, assim como a sua altura e
        largura.
        No caso a seguir, a struct BOX q foi adquirida na linha anterior,
        armazena essas informacoes referentes a cada linha identificada
        anteriormente. Desta forma, é possivel aplicar o reconhecimento,
        linha a linha.
      */
      api->SetRectangle(box->x, box->y, box->w, box->h);

      char* ocrResult = api->GetUTF8Text();

      // A funcao a seguir retorna o nível de confiança do reconhecimento realizado.
      int conf = api->MeanTextConf();

      printf("Box[%d]: x=%d, y=%d, w=%d, h=%d, Confiança: %d, text: %s",
                    i, box->x, box->y, box->w, box->h, conf, ocrResult);
      delete [] ocrResult;

    }

    // Destruindo objeto utilizado e liberando memória
    api->End();
    pixDestroy(&image);

    return 0;
}

// g++ -o ex reader_line_by_line.cpp -llept -ltesseract

# Fábrica de canetas

## Autores

&nbsp;&nbsp;&nbsp;[Felipe Aparecido da Silva](https://github.com/FehASilva) (11954502)

&nbsp;&nbsp;&nbsp;[Gabriela Passos de Andrade](https://github.com/gabipandrade) (12625142)

&nbsp;&nbsp;&nbsp;[Gustavo Carvalho Araújo](https://www.linkedin.com/in/guaraujoc/) (13735630)

&nbsp;&nbsp;&nbsp;[Henrique de Oliveira Araujo](https://github.com/HerbGlrt) (13863950)

&nbsp;&nbsp;&nbsp;[Rafael Cunha Bejes Learth](https://github.com/RafaelLearth) (13676367)

## Descrição

&nbsp;&nbsp;&nbsp;A aplicação busca simular o sistema de uma fábrica de canetas em C, com o uso do ***Pthreads***, semáforos e variáveis de condição.

&nbsp;&nbsp;&nbsp;O programa será executado em 6 *threads*, divididas em:

* Fabricação de canetas;
* Armazenamento da matéria prima e canetas produzidas;
* Compra;
* Venda.

&nbsp;&nbsp;&nbsp;Cada funcionalidade se encontra dividida em *ranks*, seguindo a ordem:

* *Rank 0*: Criador;
* *Rank 1*: Depósito de matéria prima;
* *Rank 2*: Célula de fabricação de canetas; 
* *Rank 3*: Controle;
* *Rank 4*: Depósito de canetas;
* *Rank 5*: Comprador.

&nbsp;&nbsp;&nbsp;A função **main()** será executada pela thread de *rank 0*, que irá recuperar dados de entrada e chamar a função criadora. As demais *threads* começam a executar diretamente as suas respectivas funções.

&nbsp;&nbsp;&nbsp;Após a criação, todas as *threads* serão executadas de forma independente, sendo sincronizadas por semáforos e variáveis de condições. A descrição básica e inicial da aplicação pode ser visualizada nas [redes de petri](https://github.com/guaraujoc/Sistemas-Operacionais-I/blob/main/redes.drawio), em que os números entre parênteses representam uma possível ordem de sincronização das *threads*.

## Funcionalidades

&nbsp;&nbsp;&nbsp;Os argumentos de entrada configuram a execução da aplicação desta aplicação são:
* 01 (int) - Quantidade de matéria prima existente no depósito para fabricação das canetas;
* 02 (int) - Quantidade de unidades enviadas por interação do Dep. Mat. Prima à célula de fabricação das canetas;
* 03 (int) - Tempo em segundos entre cada envio de Matéria Prima às células de fabricação;
* 04 (int) - Tempo em segundos para fabricar UMA caneta;
* 05 (int) - Quantidademáxima de canetas que podem ser armazenadas no depósito de canetas simultaneamente;
* 06 -(int) - Quantidade de canetas compradas a cada interação pelo comprador; e
* 07 - (int) - Tempo em segundos de espera entre as compras de canetas.

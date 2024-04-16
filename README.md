# Visualizador de Grafos

**Número da Lista**: 12<br>
**Conteúdo da Disciplina**: Grafos 1<br>

## Alunos
| Matrícula  | Aluno                                    |
| ---------- | ---------------------------------------- |
| 19/0118059 | Vinicius Angelo de Brito Vieira          |
| 18/0126130 | Luciano Alves do Brasil Schindel Machado |

## Sobre 
O projeto **Visualizador de Grafos** é uma aplicação desenvolvida em linguagem C que permite aos usuários interagir e visualizar grafos diretamente no terminal. Este programa é projetado para ser tanto educativo quanto funcional, oferecendo uma interface interativa onde os usuários podem inserir as coordenadas no plano cartesiano para adicionar vértices ao grafo.

# **Funcionalidades Principais:**

- **Criação de grafos de exemplo:** O programa é capaz de criar um grafo de um cubo, e também de um tetraedro, para que o usuário possa visualizar a estrutura de um grafo de forma mais simples, sem a necessidade de criar vértices manualmente.

- **Criação Interativa de Vértices:** Os usuários podem especificar as coordenadas de cada vértice no plano cartesiano. Isso é feito através de uma série de prompts no terminal, onde o usuário informa as posições (x, y) para cada vértice que deseja adicionar ao grafo.

- **Visualização de Grafos:** Uma vez que todos os vértices são inseridos, o programa utiliza o algoritmo de Busca em Profundidade (DFS - Depth First Search) para explorar e visualizar as conexões entre os vértices. O algoritmo DFS é fundamental para determinar a maneira como os vértices são conectados e como o grafo é estruturado.

- **Renderização Gráfica no Terminal:** Após a execução do DFS, o programa renderiza o grafo no terminal, mostrando visualmente a estrutura do grafo conforme especificado pelo usuário. Esta visualização ajuda na compreensão das relações entre os vértices e a estrutura geral do grafo.

O principal objetivo deste projeto é fornecer uma ferramenta simples, porém poderosa, para a visualização e entendimento de grafos. Ele visa ajudar estudantes e entusiastas em computação a entender melhor os conceitos de grafos e algoritmos de busca, como o DFS, através de uma abordagem prática e interativa.

## Screenshots

<img src="assets/images/exemplo4.jpeg" width="1024" height="500"> 
<img src="assets/images/exemplo3.jpeg" width="1024" height="500"> 
<img src="assets/images/exemplo2.jpeg" width="1024" height="500"> 
<img src="assets/images/exemplo1.jpeg" width="1024" height="500">

## Instalação 
**Linguagem**: C<br>
Para instalar o programa, é necessário o compilador GCC instalado, e a utilização do sistema operacional windows, basta clonar o repositório e executar o arquivo executável. Para isso, execute os seguintes comandos no terminal:
```bash
$ git clone https://github.com/viniciusvieira00/Grafos1_Visualizador-de-Grafos.git
```

## Uso 
Para a utilização do programa, basta executar o arquivo executável e seguir as instruções na tela, para executar o programa, execute o seguinte passo a passo na pasta que acaba de clonar o projeto:
```bash
$ cd Grafos1_Visualizador-de-Grafos/src
$ ./run.bat
```

## Outros 
É necessário ter o compilador GCC instalado para a execução do programa, e que seja compatível com a biblioteca pthreads, para isso recomendamos a utilização do compilador GNU GCC. <br>
Link para download: https://gcc.gnu.org/install/index.html

## Vídeo do Projeto

<iframe width="560" height="315" src="https://www.youtube.com/embed/RHv2X3MNMso?si=iNhAUgU52LatHqDW" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

Caso deseje acessar o vídeo pelo próprio youtube, você consegue[clicando aqui.](https://www.youtube.com/watch?v=RHv2X3MNMso).



= OpenAI APIを使った機械学習タスクの置き換え
== 背景と目的
OpenAI APIが公開されました。
これは、ChatGPTの機能をAPIを通して利用できるものです。
チャットだけでなく、従来scikit-learnなどで行っていたモデルの学習や推論を置き換えることもできます。
ここでは、それを実際に行ってみます。

== 環境構築
まず、環境構築を行います。
Macで確認していますが、Windowsでも同様に行うことができるはずです。

=== OpenAI登録
まずはブラウザからOpenAI開発者トップ(@<href>{https://platform.openai.com/overview})にアクセスします。
@<img>{higuchi_intro1}のような画面が表示されるので、"Sign up"をクリックします。
@<img>{higuchi_intro2}のような画面が表示されるので、メールアドレスとパスワードを入力すると確認メールが送信されます。
確認メール記載のボタンをクリックすれば、登録完了です。ログイン状態となります。

//indepimage[higuchi_intro1][OpenAi開発者トップ]
//indepimage[higuchi_intro2][OpenAiログイン画面]

=== OpenAI API key取得
ログインすると、@<img>{higuchi_intro3}のように右上にメニューが表示されます。
"View API Key"をクリックすると、@<img>{higuchi_intro4}のような画面が表示されます。
ここで"Create New Secret Key"をクリックすると、API Keyが発行されます。

//indepimage[higuchi_intro3][OpenAiメニュー]
//indepimage[higuchi_intro4][OpenAi API Key管理画面]

左メニューでSettingsを選ぶと、@<img>{higuchi_intro6}のような画面にOrganizationIdが表示されるので、メモしておきます。

//indepimage[higuchi_intro6][OpenAiSettings]

ここで取得した鍵を以下のように環境変数に設定しておきます。

//cmd{
$ export OPENAI_ORGANIZATION=<OrganizationId>
$ export OPENAI_API_KEY=<SecretKey>
//}

=== Anacondaインストール
Anacondaは機械学習タスクで良く使うツールをまとめたものです。
今回はJupyter Notebookで試していきます。

@<img>{higuchi_intro5}のようなダウンロード画面(@<href>{https://www.anaconda.com/download})から
Anacondaをダウンロードして、インストールします。

//indepimage[higuchi_intro5][Anacondaダウンロード]

=== サンプル取得
今回はOpenAIの公式サンプルを使って挙動を確認します。
サンプルをダウンロードします。

//cmd{
$ git clone https://github.com/openai/openai-cookbook.git
//}

これで環境構築は完了です。

== 事例
環境構築で導入したJupyter Notebookを使って、よくある機械学習タスクをOpenAI APIで行っていきます。
Anacondaをスタートすると@<img>{higuchi_bot1}のような画面が表示されるので、Jupyter NootbookのLaunchボタンをクリックします。
しばらく待つと、@<img>{higuchi_bot2}のような画面が表示されます。

//indepimage[higuchi_bot1][JupiterDirectory]
//indepimage[higuchi_bot2][JupiterDirectory]

=== 類似アイテム推薦
商品ページに類似商品を掲載しているケースがあります。
協調フィルタリングを使う実装もありますが、アイテムをvectorにして距離の近いものを表示している実装もあります。
アイテムの文字情報からvectorを生成するembedding処理をopenai APIを使うことにより、簡単に行うことができます。

まず、元となるデータを作成します。
今回は記事情報を利用して、類似記事の生成を目指します。

//indepimage[higuchi_rec2][データ作成]

それをcsvでダウンロードし、Jupiterからアクセスしやすいディレクトリにコピーします。

//cmd{
cp ~/Downloads/bquxjob_xxxx_xxxx.csv openai-cookbook/examples/data/article_list.csv
//}

Jupiter Notebookで openai-cookbook/examples/Recommendation_using_embeddings.ipynb を開きます。

一番上に必要なパッケージのインストールを追加します。

//cmd{
pip install pickle openai pandas
//}

次に認証情報の読み込みを追加します。

//cmd{
import openai
openai.organization = os.getenv("OPENAI_ORGANIZATION")
openai.api_key = os.getenv("OPENAI_API_KEY")
//}

データセットを作成したものに置き換えます。

//cmd{
dataset_path = "data/article_list.csv"
//}

Run Allすると、@<img>{higuchi_rec3}のような類似アイテムが表示されます。

//indepimage[higuchi_rec3][推論]

=== Web接客ボット
Webサイトにチャットで質問できるモーダルをたまに見ます。
Webをクロールして学習し、質問に対して回答するボットを作ります。

Webページからテキストを抜き出し、openai APIのembedding機能を使ってvectorにします。
質問も同様にvectorにして、コサイン距離が近いテキストを応答します。

Jupiter Notebookで openai-cookbook/apps/web-crawl-q-and-a/web-qa.ipynb を開きます。
これがベースとなるサンプルです。
一番上に必要なパッケージのインストールを追加します。

//cmd{
pip install tiktoken openai numpy pandas
//}

次に認証情報の読み込みを追加します。

//cmd{
import openai
openai.organization = os.getenv("OPENAI_ORGANIZATION")
openai.api_key = os.getenv("OPENAI_API_KEY")
//}

その下にクロール用のスクリプトがあるので、クロール対象を変更します。

//cmd{
# Define root domain to crawl
domain = "aumo.jp"
full_url = "https://aumo.jp/"
//}

一番下の所に質問を書きます。

//cmd{
answer_question(df, question="ゴールデンウィークの穴場スポットは？")
//}

Run Allすると、クロールして取得した情報から学習し、質問に対する回答が表示されます。

//indepimage[higuchi_bot3][推論]


== まとめ
OpenAI APIを使うことにより、モデルの学習や推論を簡単に行うことができるようになりました。
しかし、全てのML関連業務を置き換えることはできません。
何を学習させるかを調べる特徴量エンジニアリング、
学習に必要なデータを集めるデータエンジニアリング、
学習や推論をプロダクションで安定的に実行させる環境を作るMLOpsなどは引き続き重要です。
つまり、業務で利用するには使い所を選ぶと思います。

しかし、趣味の開発では大活躍しそうです。
iPhone登場時にエンジニアが思いつきでアプリを作って公開していた時期と雰囲気が似ているように感じます。
このビックウェーブに乗って、いろいろ作っていきたいと思います。

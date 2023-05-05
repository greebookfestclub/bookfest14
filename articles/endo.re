
= Android定期課金のGoogle Play Developer API v2系では無償期間が取れなくなるのか？

== 概要
この記事では、2022年5月に発表されたGoogle Playのサブスクリプション向けの新APIの、無償期間の扱いについて検討します。

== 経緯
=== 2022年以前
現在、各種プラットフォームでは課金の一環として定期課金を使えることがあります。また定期課金では定番のサービスとして、初回契約の時だけ無償で契約できるものがあります。サービス提供者は無償期間中のユーザーに対して、ログなどで把握したい、何か他と区別したメッセージを表示したいなどのニーズがあり、プラットフォーム側でも無償期間か否かを把握できるようにしてくれています。

Androidで定期課金を扱う上で、詳細情報を取得するのにサーバーAPIは欠かせません。有効期間や各種ステータスの他、定期課金がどのような状態かを調べるのに使います。今回注目するのは無償期間の判定です。2022年以前に使っていたAPI@<fn>{endofn01}ではpaymentState値を使います。

//footnote[endofn01][https://developers.google.com/android-publisher/api-ref/rest/v3/purchases.subscriptions?hl=ja]

この値は定期購入の支払い状態を示すものです、ですので無償期間を調べるにはこのAPIを使えばokでした。

※なお、この値は当初APIにありませんでしたが、2017年に追加されたものです@<fn>{endofn02}。
//footnote[endofn02][https://developers-jp.googleblog.com/2017/05/track-your-subscriptions-better-with.html]


=== 2022年以後
Androidの定期課金について、2022年5月のGoogle I/Oにて定期課金周りの考え方について大きく変更する内容が発表されました。従来は定期課金毎に設定は1通りでした、そのため定期課金請求期間やお試し期間や価格など複数種類を持ちたい場合でも、別々の定期課金を作成する必要がありました。。今後の定期課金はSubscription - Base plan - Offer という3層の枠組みになり、定期課金で何を提供するか(Subscription)に対して、期間や料金のプラン(Base Plan)、無償期間やお試し価格など(Offer)を、それぞれ複数種類設定することができるようになりました@<fn>{endofn03}。
//footnote[endofn03][https://support.google.com/googleplay/android-developer/answer/12124625?hl=ja]

このタイミングで、クライアント向けにはGoogle Play Billing Library v5@<fn>{endofn04}、サーバー向けにはGoogle Play Developer APIの定期課金API v2がそれぞれ発表されています@<fn>{endofn05}。

//footnote[endofn04][https://developer.android.com/google/play/billing/compatibility?hl=ja]
//footnote[endofn05][https://developers.google.com/android-publisher/api-ref/rest/v3/purchases.subscriptionsv2?hl=en]

なお、Google Play Billing Libraryについてはサポート期間が2年と決まっており、例えば新規アプリなら2023年8月2日以降はv5を使う必要があります。


== サーバーAPIについて
サーバー向けのGoogle Play Developer APIについては、大きく分けて定期課金プラン管理用のものと、ユーザーの定期課金情報を取得するものがあります。今回ここで検討するのはユーザーの定期課金情報を取得する方です。具体的には2022年以前のpurchases.subscriptions.get(v1 APIと呼びます)と、2022年5月登場のpurchases.subscriptionsv2.get(v2 APIと呼びます)について見ていきます。
v1からv2への変更ではGoogleの新しい定期課金についての機能追加や修正が入っています。v1では定期課金情報に親子関係は無いため単なるkey-valueの塊として全情報が返ってきますが、v2ではある程度構造がある上に情報が追加されています。

さて、ユーザーの定期課金の状態は以下の値から取得できます。ここで問題になるのが定期課金の無料期間に関する情報取得です。v1からv2に変わったことで無償期間が取得できなくなっています。

 * v1 API
 ** paymentState
 *** (適当に訳) 定期課金の支払いの状態。
 *** ステータス : 支払い保留、支払い済み、無料期間、アップグレードもしくはダウングレードの保留
 * v2 API
 ** subscriptionState
 *** (適当に訳) 定期課金の状態。例えば、有効かキャンセル済みか。
 *** ステータス : 不明、支払い待ち、有効、一時停止、支払い猶予期間、保留中、キャンセル、有効期限切れ

== サーバーAPI v2から無償期間が取得できない

新しい定期課金への移行ドキュメント@<fn>{endofn06}では、当初は無償期間の取得はv2 APIでは「coming soon」扱いになっていました。しかし、paymentState値についての記述がいつの間にか変わり、「coming soon」が消えて取得できるようになる見込みが無くなってしいました。
//footnote[endofn06][https://developer.android.com/google/play/billing/compatibility?hl=ja]

//indepimage[v1api_paymentstate_1][画像 2022-09-30時点]

//indepimage[v1api_paymentstate_2][画像 2022-11-24時点]


すなわち、無償期間に関する値はv2 APIで無くなってしまっています。


== 解決策

これで困ったのでもろもろ確認をしました。最終的には各自問い合わせて頂いた方が良さそうですが、各種資料から読みった内容などをまとめると以下の内容になります。

 * クライアントSDKの方は2年で移行が必要だが、APIの移行期間は2年という訳では無い、
 ** なのでv1 APIを継続して使用してもしばらくは問題無い
 ** つまり、Billing Library 5.0に移行後もpurchases.subscriptionsv1をとりあえずは使う
 * v2 APIで無償期間を取れるようにするかの対応は検討するらしい

ただし、v1 APIの利用継続はそれはそれで問題があります。
 * v1 APIは新しい定期課金の概念に合致しないので、旧定期課金仕様と互換性のある定期課金のみが実施できる状態になる
 ** google play console的には「下位互換性」と表記されている状態にする必要があります
 ** それぞれの定期購入に対して基本プランと特典がそれぞれ1つだけの状態にする必要があります
 * v1 APIには無償期間かつ支払猶予期間が表現されていないので取得できない問題があるが、それが継続する

また、代替案としてはおそらくofferTagsを使う方法があるようです。無償期間はofferのいくつかの種類のうち1つになるので、ここに無償期間の情報が入ってくる、それを取得する方法です。複数のofferを設定する場合などは、v2 APIを使った上でこの値を参照することになると思われます。ただ、細かい挙動がドキュメントに書いていないため、まだ十分な知見は無いようです@<fn>{endofn07}。
//footnote[endofn07][https://developers.google.com/android-publisher/api-ref/rest/v3/purchases.subscriptionsv2#offerdetails]


== まとめ

新しいv2 APIは、APIの値やドキュメントの方は微妙に整備されていないなど、慎重にならざるを得ない部分も残っています。ですがgoogleの新しい定期課金はかなりの柔軟さを持てそうな新しい仕組みで、上手く使えば複雑になりそうな定期課金を管理するために必須になるでしょう。問い合わせなどしなければならないとはいえ今後もウォッチしていきまそう。

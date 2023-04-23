
= Android定期課金のGoogle Play Developer API v2系では無償期間が取れなくなるのか？

== 概要
この記事では、2022年5月に発表されたGoogle Playのサブスクリプション向けの新APIの、無償期間の扱いについて検討します。

== 経緯
=== 2022年以前
現在、各種プラットフォームでは課金の一環として定期課金を使えることがあります。また定期課金では定番のサービスとして、初回契約の時だけ無償で契約できるものがあります。サービス提供者は無償期間中のユーザーに対して、ログなどで把握したい、何か他と区別したメッセージを表示したいなどのニーズがあり、プラットフォーム側でも無償期間か否かを把握できるようにしてくれています。

Androidで定期課金を扱う上で、詳細情報を取得するのにサーバーAPIは欠かせません。有効期間や各種ステータスの他、定期課金がどのような状態かを調べるのに使います。今回注目するのはpaymentState値の無償期間の判定です。当初はこちらのAPIを利用していました、便宜上v1 APIと呼びます。
https://developers.google.com/android-publisher/api-ref/rest/v3/purchases.subscriptions?hl=ja
この値は定期購入の支払い状態を示すもので、0:支払い保留中, 1:支払い受領済み, 2:無料試用サービス、を返します。
ですので無償期間を調べるにはこのAPIを使えばokでした。

※なお、この値は当初APIにありませんでしたが、2017年に追加されたものです。
https://developers-jp.googleblog.com/2017/05/track-your-subscriptions-better-with.html


=== 2022年以後
Androidの定期課金について、2022年5月のGoogle I/Oにて定期課金周りの考え方について大きく変更する内容が発表されました。従来は定期課金毎に設定は1通りでした、そのため定期課金請求期間やお試し期間や価格など複数種類を持ちたい場合でも、別々の定期課金を作成する必要がありました。。今後の定期課金はSubscription - Base plan - Offer という3層の枠組みになり、定期課金で何を提供するか(Subscription)に対して、期間や料金のプラン(Base Plan)、無償期間やお試し価格など(Offer)を、それぞれ複数種類設定することができるようになりました。
https://support.google.com/googleplay/android-developer/answer/12124625?hl=ja





TODO ここまで書いた







このタイミングで、クライアント向けにはGoogle Play Billing Library v5、サーバー向けにはGoogle Play Developer APIの定期課金API v2がそれぞれ発表されています。

(リンク)
Google Play Developer API 新APIやv2系の追加
https://developer.android.com/google/play/billing/compatibility?hl=ja
(リンク)
https://developers.google.com/android-publisher/api-ref/rest/v3/purchases.subscriptionsv2


TODO v1やv2でなぜこれを使うのか
v2 APIは無償期間が取得できませんが、それに該当するpaymentState値による無償期間値はcoming soon扱いになっていました。
しかし、paymentState値についての記述がいつの間にか変わっていました。

//indepimage[v1api_paymentstate_1][画像 2022-09-30時点]

//indepimage[v1api_paymentstate_2][画像 2022-11-24時点]



coming soonがなくなりました。
ここから読み取れることは、無償期間に関する値はpaymentState値では返さないという意思表示です。

もろもろ確認を進めると、
・クライアントSDKの方は2年で移行が必要だが、APIの移行期間は2年という訳ではい
・なのでv1 APIを使っていってもとりあえず問題ない
つまりBilling Library 5.0に移行後もpurchases.subscriptionsv1をとりあえずは使う
・v2 APIでの対応は検討とのこと
アプリごとにどのように影響するか異なるため、影響ある場合は確認するのが良いでしょう


ただし、paymentState値で無償期間が取得できるとすると、それはそれで問題があります。
1 無償期間かつ支払猶予期間が表現できない
従来のAPIでもそうでした
2 新しい定期課金の概念に合致しない
無償期間かどうかは定期課金そのものの性質ではなく、定期課金に付随する各種プランの1つという位置付けになっています。そのためここで無償期間かどうかを返すのは不適切と考えている可能性があります。

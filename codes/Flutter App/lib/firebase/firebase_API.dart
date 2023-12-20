// ignore: file_names
import 'package:firebase_messaging/firebase_messaging.dart';
import 'package:lora/main.dart';

class FirebaseApi {
  // create an instance of firebase messaging
  final _firebaseMessaging = FirebaseMessaging.instance;
  // function to initialized notification
  Future<void> initNotification() async {
    // request permission from user
    await _firebaseMessaging.requestPermission();
    // fetch the FCM token for this device
    final fCMToken = await _firebaseMessaging.getToken();
    // print the token
    print('Tocken: $fCMToken');

    initNotification();
  }

  // functio to handel receved notification
  void handleMessage(RemoteMessage? message) {
    if (message == null) return;

    navigatorKey.currentState?.pushNamed(
      '/Notification_screen',
      arguments: message,
    );
  }
  // function to initialized forground and background settings
}

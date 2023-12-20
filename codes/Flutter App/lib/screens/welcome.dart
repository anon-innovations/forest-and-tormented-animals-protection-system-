import 'package:flutter/material.dart';
import 'package:lora/login/LoginScreen.dart';
import 'package:lottie/lottie.dart';
import 'package:animate_do/animate_do.dart';

class Welcome extends StatelessWidget {
  final Duration duration = const Duration(milliseconds: 800);

  const Welcome({Key? key}) : super(key: key);

  void _onWelcomeEnd(context) {
    Navigator.push(
      context,
      MaterialPageRoute(builder: (context) => const SigningPageState()),
    );
  }

  @override
  Widget build(BuildContext context) {
    final size = MediaQuery.of(context).size;
    return Scaffold(
      backgroundColor: const Color.fromARGB(255, 239, 239, 239),
      body: Container(
        margin: const EdgeInsets.all(8),
        width: size.width,
        height: size.height,
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            ///
            FadeInUp(
              duration: duration,
              delay: const Duration(milliseconds: 2000),
              child: Container(
                margin: const EdgeInsets.only(
                  top: 150,
                  left: 5,
                  right: 5,
                ),
                width: size.width,
                height: 200,
                child: Lottie.asset("assets/welcome.json", animate: true),
              ),
            ),

            ///
            const SizedBox(
              height: 40,
            ),

            /// TITLE
            FadeInUp(
              duration: duration,
              delay: const Duration(milliseconds: 1600),
              child: const Text(
                "echoSentry",
                style: TextStyle(
                    color: Colors.black,
                    fontSize: 35,
                    fontWeight: FontWeight.bold),
              ),
            ),

            ///
            const SizedBox(
              height: 20,
            ),

            /// SUBTITLE
            FadeInUp(
              duration: duration,
              delay: const Duration(milliseconds: 1000),
              child: const Text(
                "Welcome to our IoT-functional smart wild life security controller app!",
                textAlign: TextAlign.center,
                style: TextStyle(
                    height: 1.2,
                    color: Colors.grey,
                    fontSize: 17,
                    fontWeight: FontWeight.w300),
              ),
            ),
            const SizedBox(
              height: 60,
            ),

            OutlinedButton(
              style: OutlinedButton.styleFrom(
                shape: const StadiumBorder(),
                side: const BorderSide(color: Colors.blueAccent),
                textStyle:
                    const TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
                animationDuration: const Duration(microseconds: 1000),
                fixedSize: const Size(150, 50),
              ),
              onPressed: () async {
                _onWelcomeEnd(context);
              },
              child: const Text('GO'),
            ),
          ],
        ),
      ),
    );
  }
}

import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:lora/screens/home_page.dart';
import 'package:lottie/lottie.dart';
import 'signup.dart';

const kTextFieldDecoration = InputDecoration(
  hintText: 'Enter a value',
  hintStyle: TextStyle(color: Color.fromARGB(255, 2, 80, 32)),
  contentPadding: EdgeInsets.symmetric(vertical: 10.0, horizontal: 20.0),
  border: OutlineInputBorder(
    borderRadius: BorderRadius.all(Radius.circular(32.0)),
  ),
  enabledBorder: OutlineInputBorder(
    borderSide: BorderSide(color: Color.fromARGB(255, 4, 134, 32), width: 1.0),
    borderRadius: BorderRadius.all(Radius.circular(32.0)),
  ),
  focusedBorder: OutlineInputBorder(
    borderSide: BorderSide(color: Color.fromARGB(255, 5, 228, 61), width: 2.0),
    borderRadius: BorderRadius.all(Radius.circular(32.0)),
  ),
);

final FirebaseAuth _auth = FirebaseAuth.instance;

class SigningPageState extends StatefulWidget {
  const SigningPageState({super.key});
  @override
  // ignore: library_private_types_in_public_api
  _SigningPageState createState() => _SigningPageState();
}

class _SigningPageState extends State<SigningPageState> {
  final TextEditingController _emailController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();
  int _sucess = 1;

  void _singIn() async {
    final User? user = (await _auth.signInWithEmailAndPassword(
            email: _emailController.text, password: _passwordController.text))
        .user;

    if (user != null) {
      setState(() {
        _sucess = 2;
      });
    } else {
      setState(() {
        _sucess = 3;
      });
    }
    if (_sucess == 2) {
      // ignore: use_build_context_synchronously
      Navigator.push(
        context,
        MaterialPageRoute(builder: (context) => const HomePage()),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        resizeToAvoidBottomInset: false,
        body: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            const SizedBox(
              height: 100,
            ),
            Container(
              alignment: Alignment.center,
              height: 150,
              child: Lottie.asset("assets/login.json", animate: true),
            ),
            const SizedBox(
              height: 20,
            ),
            Stack(
              children: <Widget>[
                Container(
                  padding: const EdgeInsets.symmetric(horizontal: 30.0),
                  child: const Text("Welcome",
                      style: TextStyle(
                          fontSize: 30,
                          fontWeight: FontWeight.bold,
                          color: Color.fromARGB(255, 2, 95, 30))),
                  alignment: Alignment.center,
                )
              ],
            ),
            const SizedBox(
              height: 20,
            ),
            Container(
              padding: const EdgeInsets.symmetric(horizontal: 30.0),
              child: Column(
                children: <Widget>[
                  TextField(
                      controller: _emailController,
                      decoration: kTextFieldDecoration.copyWith(
                          hintText: 'Enter your email')),
                  const SizedBox(
                    height: 20,
                  ),
                  TextField(
                      controller: _passwordController,
                      obscureText: true,
                      decoration: kTextFieldDecoration.copyWith(
                          hintText: 'Enter your Password')),
                  const SizedBox(
                    height: 48,
                  ),
                  OutlinedButton(
                    style: OutlinedButton.styleFrom(
                      shape: const StadiumBorder(),
                      side: const BorderSide(
                          color: Color.fromARGB(255, 3, 92, 6)),
                      textStyle: const TextStyle(
                          fontSize: 16, fontWeight: FontWeight.bold),
                      animationDuration: const Duration(microseconds: 1000),
                      fixedSize: const Size(150, 50),
                    ),
                    onPressed: () async {
                      _singIn();
                    },
                    child: const Text('LOGIN'),
                  ),
                  const SizedBox(
                    height: 20,
                  ),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: <Widget>[
                      InkWell(
                        onTap: () {
                          Navigator.push(
                            context,
                            MaterialPageRoute(
                                builder: (context) => const SignupPage()),
                          );
                        },
                        child: const Text(
                          'Register',
                          style: TextStyle(
                              color: Color.fromARGB(255, 3, 90, 29),
                              fontFamily: 'Montserrat',
                              fontWeight: FontWeight.bold,
                              fontSize: 20,
                              decoration: TextDecoration.underline),
                        ),
                      )
                    ],
                  ),
                  /*const SizedBox(
                    height: 15,
                  ),
                  Container(
                      alignment: Alignment.center,
                      padding: const EdgeInsets.symmetric(horizontal: 16),
                      child: Text(_sucess == 1 ? '' : (_sucess == 2 ? 'Successfully Signed in $_userEmail': 'Sign in failed'),
                        style: const TextStyle(color: Colors.red),
                      )
                  ),*/
                ],
              ),
            )
          ],
        ));
  }
}

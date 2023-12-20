// ignore: file_names
import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:lottie/lottie.dart';

final FirebaseAuth _auth = FirebaseAuth.instance;

const kTextFieldDecoration = InputDecoration(
  hintText: 'Enter a value',
  hintStyle: TextStyle(color: Color.fromARGB(255, 2, 88, 31)),
  contentPadding: EdgeInsets.symmetric(vertical: 10.0, horizontal: 20.0),
  border: OutlineInputBorder(
    borderRadius: BorderRadius.all(Radius.circular(32.0)),
  ),
  enabledBorder: OutlineInputBorder(
    borderSide: BorderSide(color: Color.fromARGB(255, 6, 126, 46), width: 1.0),
    borderRadius: BorderRadius.all(Radius.circular(32.0)),
  ),
  focusedBorder: OutlineInputBorder(
    borderSide: BorderSide(color: Color.fromARGB(255, 19, 233, 73), width: 2.0),
    borderRadius: BorderRadius.all(Radius.circular(32.0)),
  ),
);

class SignupPage extends StatefulWidget {
  const SignupPage({super.key});

  @override
  // ignore: library_private_types_in_public_api
  _SignupPageState createState() => _SignupPageState();
}

class _SignupPageState extends State<SignupPage> {
  final TextEditingController _emailController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();

  // ignore: unused_element
  void _register() async {
    final User? user = (await _auth.createUserWithEmailAndPassword(
            email: _emailController.text, password: _passwordController.text))
        .user;

    if (user != user) {
      setState(() {});
    } else {
      setState(() {});
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
              child: Lottie.asset("assets/register.json", animate: true),
            ),
            const SizedBox(
              height: 20,
            ),
            Stack(
              children: <Widget>[
                Container(
                  padding: const EdgeInsets.symmetric(horizontal: 30.0),
                  alignment: Alignment.center,
                  child: const Text("Create Account",
                      style: TextStyle(
                          fontSize: 30,
                          fontWeight: FontWeight.bold,
                          color: Color.fromARGB(255, 3, 102, 33))),
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
                          color: Color.fromARGB(255, 3, 109, 26)),
                      textStyle: const TextStyle(
                          fontSize: 16, fontWeight: FontWeight.bold),
                      animationDuration: const Duration(microseconds: 1000),
                      fixedSize: const Size(150, 50),
                    ),
                    onPressed: () async {
                      _register();
                    },
                    child: const Text('SUBMIT'),
                  ),
                  const SizedBox(
                    height: 25,
                  ),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: <Widget>[
                      InkWell(
                        onTap: () {
                          Navigator.of(context).pop();
                        },
                        child: const Text(
                          'Login',
                          style: TextStyle(
                              fontSize: 20,
                              color: Color.fromARGB(255, 5, 145, 35),
                              fontFamily: 'Montserrat',
                              fontWeight: FontWeight.bold,
                              decoration: TextDecoration.underline),
                        ),
                      )
                    ],
                  )
                ],
              ),
            )
          ],
        ));
  }
}

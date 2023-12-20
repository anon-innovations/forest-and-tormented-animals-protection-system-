import 'package:flutter/material.dart';
import 'package:lottie/lottie.dart';

class ImageChangeDemo extends StatefulWidget {
  const ImageChangeDemo({super.key});

  @override
  _ImageChangeDemoState createState() => _ImageChangeDemoState();
}

class _ImageChangeDemoState extends State<ImageChangeDemo> {
  String _selectedOption = 'Option 1';

  void _changeImage(String? option) {
    if (option != null) {
      setState(() {
        _selectedOption = option;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Image Change Demo'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Lottie.asset(
              _selectedOption == 'Option 1'
                  ? 'assets/welcome.json'
                  : _selectedOption == 'Option 2'
                      ? 'assets/bear.json'
                      : _selectedOption == 'Option 3'
                          ? 'assets/fire.json'
                          : _selectedOption == 'Option 4'
                              ? 'assets/tiger.json'
                              : 'assets/weapon.json',
              width: 200,
              height: 200,
            ),
            SizedBox(height: 20),
            Column(
              children: <Widget>[
                for (int i = 1; i <= 5; i++)
                  Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: <Widget>[
                      Radio(
                        value: 'Option $i',
                        groupValue: _selectedOption,
                        onChanged: _changeImage,
                      ),
                      Text('Option $i'),
                    ],
                  ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}

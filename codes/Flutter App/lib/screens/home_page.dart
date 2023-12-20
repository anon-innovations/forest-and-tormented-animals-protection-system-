import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:lora/test/map.dart';
import 'package:lottie/lottie.dart';

final DatabaseReference database = FirebaseDatabase.instance.reference();

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: StreamBuilder<DatabaseEvent>(
        stream: database.child('com').onValue,
        builder: (BuildContext context, AsyncSnapshot<DatabaseEvent> snapshot) {
          if (snapshot.hasError) {
            return Text('Error: ${snapshot.error}');
          }

          if (snapshot.connectionState == ConnectionState.waiting) {
            return const Text('Loading...');
          }

          if (!snapshot.hasData || snapshot.data == null) {
            return const Text('No data found.');
          }

          final DataSnapshot data = snapshot.data!.snapshot;

          if (!data.exists) {
            return const Text('No sensor data available.');
          }

          final dynamic sensorData = data.value;
          if (sensorData is Map) {
            final int? device = sensorData['Device'] as int?;
            final int? count = sensorData['Count'] as int?;
            final String? detection = sensorData['Detection'] as String?;

            String selectedOption = '$detection';

            if (device != null) {
              return Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Container(
                    alignment: Alignment.center,
                    padding: const EdgeInsets.symmetric(horizontal: 15),
                    child: const Text(
                      "Detection \n Alert ",
                      textAlign: TextAlign.center,
                      style: TextStyle(
                        color: Colors.indigo,
                        fontSize: 38,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                  ),
                  const SizedBox(
                    height: 30,
                  ),
                  Container(
                    margin: const EdgeInsets.only(
                      top: 15,
                      left: 5,
                      right: 5,
                    ),
                    width: 200,
                    height: 200,
                    child: Lottie.asset(
                      selectedOption == 'Elephant'
                          ? 'assets/welcome.json'
                          : selectedOption == 'Bear'
                              ? 'assets/bear.json'
                              : selectedOption == 'fire'
                                  ? 'assets/fire.json'
                                  : selectedOption == 'Fire'
                                      ? 'assets/fire.json'
                                      : selectedOption == 'Leopard'
                                          ? 'assets/tiger.json'
                                          : selectedOption == 'Gunshot'
                                              ? 'assets/gunshot.json'
                                              : selectedOption == 'Weapon'
                                                  ? 'assets/weapon.json'
                                                  : 'assets/search.json',
                      width: 200,
                      height: 200,
                    ),
                  ),
                  const SizedBox(
                    height: 30,
                  ),
                  Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Row(
                        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        children: [
                          Text(
                            '$count $detection detected',
                            style: const TextStyle(
                              fontSize: 20,
                              fontWeight: FontWeight.w500,
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
                  const SizedBox(
                    height: 20,
                  ),
                  Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          const Text(
                            'In device: ',
                            style: TextStyle(
                              fontSize: 20,
                              fontWeight: FontWeight.w500,
                            ),
                          ),
                          const SizedBox(
                            height: 50,
                          ),
                          Text(
                            '$device',
                            style: const TextStyle(
                              fontSize: 20,
                              fontWeight: FontWeight.w500,
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
                  const SizedBox(
                    height: 30,
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
                      Navigator.push(
                        context,
                        MaterialPageRoute(
                            builder: (context) => const mapclass()),
                      );
                    },
                    child: const Text('Location'),
                  ),
                ],
              );
            }
          }
          return const Text('Invalid sensor data format.');
        },
      ),
    );
  }
}

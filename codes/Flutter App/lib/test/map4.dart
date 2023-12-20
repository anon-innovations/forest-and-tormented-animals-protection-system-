import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'package:firebase_database/firebase_database.dart';

class mapclass4 extends StatefulWidget {
  @override
  _MapWithMarkersState createState() => _MapWithMarkersState();
}

class _MapWithMarkersState extends State<mapclass4> {
  final DatabaseReference _databaseReference =
      FirebaseDatabase.instance.reference().child('location');

  final List<LatLng> locations = [];
  final List<Marker> _markers = [];

  @override
  void initState() {
    super.initState();

    _databaseReference.onValue.listen((event) {
      // Use the 'event.snapshot.value' to access the data
      var dataSnapshot = event.snapshot.value;

      if (dataSnapshot != null && dataSnapshot is Map<dynamic, dynamic>) {
        locations.clear();

        dataSnapshot.forEach((key, value) {
          if (value is Map<dynamic, dynamic>) {
            double latitude = (value['latitude'] as double?) ?? 0.0;
            double longitude = (value['longitude'] as double?) ?? 0.0;
            if (kDebugMode) {
              print('Device: $key, Latitude: $latitude, Longitude: $longitude');
            }
            _markers.add(
              Marker(
                markerId: MarkerId(key),
                position: LatLng(latitude, longitude),
                infoWindow: InfoWindow(
                  title: key, // Location name
                ),
              ),
            );
          }
        });

        setState(() {
          // Update the UI when data changes
        });
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: const Text('Device Location Map'),
        ),
        body: GoogleMap(
          initialCameraPosition: const CameraPosition(
            target: LatLng(6.917982489542783,
                79.97341812856874), // Initial camera position
            zoom: 13.0, // Adjust the zoom level as needed
          ),
          markers: Set<Marker>.from(_markers),
        ));
  }
}

import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'package:lora/test/map_util.dart';

class MapScreen extends StatefulWidget {
  const MapScreen({super.key});

  @override
  // ignore: library_private_types_in_public_api
  _MapScreenState createState() => _MapScreenState();
}

class _MapScreenState extends State<MapScreen> {
  late GoogleMapController mapController;

  Set<Marker> _markers = {};

  @override
  void initState() {
    super.initState();
    _fetchMarkers();
  }

  void _fetchMarkers() async {
    List<MarkerData> markerList = await getMarkerCoordinates();

    setState(() {
      _markers = markerList.map((markerData) {
        return Marker(
          markerId: MarkerId(markerData.toString()),
          position: LatLng(markerData.latitude, markerData.longitude),
          icon: BitmapDescriptor.defaultMarker,
        );
      }).toSet();
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Map with Markers'),
      ),
      body: GoogleMap(
        onMapCreated: (GoogleMapController controller) {
          mapController = controller;
        },
        initialCameraPosition: const CameraPosition(
          target: LatLng(0.0, 0.0), // Set the initial camera position
          zoom: 10.0, // You can adjust the initial zoom level
        ),
        markers: _markers,
      ),
    );
  }
}

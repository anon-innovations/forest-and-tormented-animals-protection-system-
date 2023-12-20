import 'package:firebase_database/firebase_database.dart';

class MarkerData {
  final double latitude;
  final double longitude;

  MarkerData(this.latitude, this.longitude);
}

// Function to retrieve marker coordinates from Firebase
Future<List<MarkerData>> getMarkerCoordinates() async {
  final databaseReference = FirebaseDatabase.instance.ref();
  DataSnapshot dataSnapshot = (await databaseReference.once()) as DataSnapshot;

  List<MarkerData> markerList = [];

  if (dataSnapshot.value != null) {
    Map<String, dynamic> data = dataSnapshot.value as Map<String, dynamic>;
    data.forEach((key, value) {
      double latitude = value['latitude'];
      double longitude = value['longitude'];
      markerList.add(MarkerData(latitude, longitude));
    });
  }
  return markerList;
}

import 'package:flutter/material.dart';

class InputCard extends StatelessWidget {
  final String title;

  final TextEditingController controller1;
  final String label1;

  final TextEditingController? controller2;
  final String? label2;
  final bool obscureText2;

  final VoidCallback onSave;

  const InputCard({
    super.key,
    required this.title,
    required this.controller1,
    required this.label1,
    this.controller2,
    this.label2,
    this.obscureText2 = false,
    required this.onSave,
  });

  @override
  Widget build(BuildContext context) {
    return Column(
    // return Card(
      // margin: const EdgeInsets.symmetric(vertical: 8),
      // child: Padding(
        // padding: const EdgeInsets.all(12),
        // child: Column(

          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              title,
              style: const TextStyle(fontSize: 14),
            ),
            const SizedBox(height: 8),

            TextField(
              controller: controller1,
              style: const TextStyle(fontSize: 14),
              decoration: InputDecoration(
                labelText: label1,
                labelStyle: const TextStyle(fontSize: 14),
                border: const OutlineInputBorder(),
                isDense: true,
                contentPadding: const EdgeInsets.symmetric(vertical: 12, horizontal: 12),
              ),
            ),
            const SizedBox(height: 8),

            if (controller2 != null)
              TextField(
                controller: controller2,
                // obscureText: obscureText2,
                style: const TextStyle(fontSize: 14),
                decoration: InputDecoration(
                  labelText: label2,
                  labelStyle: const TextStyle(fontSize: 14),
                  border: const OutlineInputBorder(),
                  isDense: true,
                  contentPadding: const EdgeInsets.symmetric(vertical: 12, horizontal: 12),
                ),
              ),
            const SizedBox(height: 8),

            // Align(
            //   alignment: Alignment.centerRight,
            //   child: ElevatedButton(
            //     onPressed: onSave,
            //     style: ElevatedButton.styleFrom(
            //       padding: const EdgeInsets.symmetric(vertical: 12, horizontal: 12),
            //       textStyle: const TextStyle(fontSize: 14),
            //     ),
            //     child: const Text("Save"),
            //   ),
            // ),
            SizedBox(
              width: double.infinity,
              child: ElevatedButton(
                onPressed: onSave,
                style: ElevatedButton.styleFrom(
                  padding: const EdgeInsets.symmetric(vertical: 12),
                  textStyle: const TextStyle(fontSize: 14),
                ),
                child: const Text("Save"),
              ),
            ),
          ],
        // ),
      // ),
    );
  }
}
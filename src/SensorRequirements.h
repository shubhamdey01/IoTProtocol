#ifndef SENSOR_REQUIREMENTS_H
#define SENSOR_REQUIREMENTS_H

// Priority requirement
enum Priority {
    PRIORITY_LOW,
    PRIORITY_HIGH
};

// Reliability requirement
enum Reliability {
    RELIABILITY_LOW,
    RELIABILITY_HIGH
};

// Type of information to transmit
enum PayloadMode {
    PAYLOAD_RAW,
    PAYLOAD_SEMANTIC
};

#endif
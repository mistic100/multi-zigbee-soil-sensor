import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    zigbeeModel: ['PlantsSensor'],
    model: 'PlantsSensor',
    vendor: 'StrangePlanet',
    description: 'Multi plants soil sensor',
    extend: [
        m.deviceEndpoints({ "endpoints": { "1": 1, "2": 2, "3": 3, "4": 4 } }),
        m.humidity({ "endpointNames": ["1", "2", "3", "4"] }),
    ],
};

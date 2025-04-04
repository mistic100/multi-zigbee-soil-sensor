import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    zigbeeModel: ['PlantsSensor'],
    model: 'PlantsSensor',
    vendor: 'StrangePlanet',
    description: 'Multi plants soil sensor',
    extend: [
        m.deviceEndpoints({ "endpoints": { "1": 10, "2": 11, "3": 12, "4": 13 } }),
        m.humidity({ "endpointNames": ["1", "2", "3", "4"] }),
    ],
    meta: { "multiEndpoint": true },
};

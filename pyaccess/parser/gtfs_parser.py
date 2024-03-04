from typing import Any
import json
import pandas as pd
import numpy as np
from shapely import contains_xy, MultiPolygon, Polygon

from .._pyaccess_ext import Node, Coord, NodeVector, Connection, ConnectionVector


class GTFSService:
    __slots__ = ["service_id", "days"]
    service_id: int
    days: list[int]

    def __init__(self, service_id, days: list[int]):
        self.service_id = service_id
        self.days = days

    def get_days(self):
        return self.days

def read_calendar(path: str) -> dict[int, GTFSService]:
    frame = pd.read_csv(f"{path}/calendar.txt")
    service_ids = frame["service_id"]
    monday = frame["monday"]
    tuesday = frame["tuesday"]
    wednesday = frame["wednesday"]
    thursday = frame["thursday"]
    friday = frame["friday"]
    saturday = frame["saturday"]
    sunday = frame["sunday"]
    services: dict[int, GTFSService] = {}
    for i in range(service_ids.size):
        service_id = int(service_ids[i])
        days = []
        if monday[i] == 1:
            days.append(1)
        if tuesday[i] == 1:
            days.append(2)
        if wednesday[i] == 1:
            days.append(3)
        if thursday[i] == 1:
            days.append(4)
        if friday[i] == 1:
            days.append(5)
        if saturday[i] == 1:
            days.append(6)
        if sunday[i] == 1:
            days.append(7)
        services[service_id] = GTFSService(service_id, days)
    return services

class GTFSStop:
    __slots__ = ["stop_id", "lon", "lat", "typ", "parent_id"]
    stop_id: int
    lon: float
    lat: float
    typ: int
    parent_id: int

    def __init__(self, id, lon, lat, typ, parent):
        self.stop_id = id
        self.lat = lat
        self.lon = lon
        self.typ = typ
        self.parent_id = parent

    def has_parant(self) -> bool:
        return self.typ >= 3

    def get_lon_lat(self) -> tuple[float, float]:
        return self.lon, self.lat

def read_stop_locations(path: str, filter: MultiPolygon) -> dict[int, GTFSStop]:
    stops_frame = pd.read_csv(f"{path}/stops.txt")
    stop_ids = stops_frame["stop_id"]
    stop_lon = stops_frame["stop_lon"]
    stop_lat = stops_frame["stop_lat"]
    stop_parents = stops_frame["parent_station"]
    location_type = stops_frame["location_type"]
    stops: dict[int, GTFSStop] = {}
    for i in range(stop_ids.size):
        id = int(stop_ids[i])
        lon = stop_lon[i]
        lat = stop_lat[i]
        parent = stop_parents[i]
        typ = location_type[i]
        if np.isnan(lon) or np.isnan(lat) or typ >= 3:
            if np.isnan(parent):
                continue
            stops[id] = GTFSStop(id, 0, 0, int(typ), int(parent))
        else:
            lon = float(lon)
            lat = float(lat)
            if not contains_xy(filter, lon, lat):
                continue
            stops[id] = GTFSStop(id, lon, lat, 0, 0)

    delete = []
    for id, stop in stops.items():
        if stop.lat == 0:
            if stop.parent_id not in stops:
                delete.append(id)
                continue
            parent_stop = stops[stop.parent_id]
            stop.lon = parent_stop.lon
            stop.lat = parent_stop.lat
    for d in delete:
        del stops[d]
    return stops


class GTFSTripStop:
    __slots__ = ["stop_id", "arrival", "departure", "sequence"]
    stop_id: int
    arrival: int
    departure: int
    sequence: int

    def __init__(self, stop_id, arival, departure, sequence):
        self.stop_id = stop_id
        self.arrival = arival
        self.departure = departure
        self.sequence = sequence


class GTFSTrip:
    __slots__ = ["trip_id", "route_id", "service_id", "stops"]
    trip_id: int
    route_id: int
    service_id: int
    stops: list[GTFSTripStop]

    def __init__(self, trip_id):
        self.trip_id = trip_id
        self.route_id = -1
        self.service_id = -1
        self.stops = []

    def set_service_id(self, service_id):
        self.service_id = service_id

    def set_route_id(self, route_id):
        self.route_id = route_id

    def add_stop(self, stop: GTFSTripStop):
        self.stops.append(stop)

    def order_stops(self):
        self.stops.sort(key=lambda x: x.sequence)

def parse_time(time_str: str) -> int:
    tokens = time_str.split(":")
    time = 0
    time += int(tokens[2])
    time += int(tokens[1]) * 60
    time += int(tokens[0]) * 3600
    return time

def read_trips(path: str, stops: dict[int, GTFSStop], services: dict[int, GTFSService]) -> dict[int, GTFSTrip]:
    trips: dict[int, GTFSTrip] = {}
    
    times_frame = pd.read_csv(f"{path}/stop_times.txt")
    trip_ids = times_frame["trip_id"]
    arrival_times = times_frame["arrival_time"]
    departure_times = times_frame["departure_time"]
    stop_ids = times_frame["stop_id"]
    stop_sequences = times_frame["stop_sequence"]
    for i in range(trip_ids.size):
        trip_id = int(trip_ids[i])
        if trip_id not in trips:
            trips[trip_id] = GTFSTrip(trip_id)
        trip = trips[trip_id]
        s_id = int(stop_ids[i])
        if s_id not in stops:
            continue
        a_time = parse_time(arrival_times[i])
        d_time = parse_time(departure_times[i])
        s_seq = int(stop_sequences[i])
        trip.add_stop(GTFSTripStop(s_id, a_time, d_time, s_seq))
    for trip in trips.values():
        trip.order_stops()

    frame = pd.read_csv(f"{path}/trips.txt")
    trip_ids = frame["trip_id"]
    route_ids = frame["route_id"]
    service_ids = frame["service_id"]
    for i in range(trip_ids.size):
        trip_id = int(trip_ids[i])
        if trip_id not in trips:
            continue
        trip = trips[trip_id]
        route_id = int(route_ids[i])
        trip.set_route_id(route_id)
        service_id = int(service_ids[i])
        if service_id not in services:
            continue
        trip.set_service_id(service_id)

    return trips


# class TransitStop:
#     __slots__ = ["stop_id", "lon", "lat", "neighbours"]
#     stop_id: int
#     lon: float
#     lat: float
#     # neighbour -> [(day, departure, arival), ...]
#     neighbours: dict[int, list[tuple[int, int, int]]]

#     def __init__(self, stop_id, lon, lat):
#         self.stop_id = stop_id
#         self.lon = lon
#         self.lat = lat
#         self.neighbours = {}

#     def add_neighbour(self, other_id: int, days: list[int], dep: int, ar: int):
#         for day in days:
#             d_neigh = self.neighbours
#             if other_id not in d_neigh:
#                 d_neigh[other_id] = []
#             trips = d_neigh[other_id]
#             trips.append((day, dep, ar))


def build_transit_graph(trips: dict[int, GTFSTrip], stops: dict[int, GTFSStop], services: dict[int, GTFSService]) -> tuple[NodeVector, ConnectionVector, dict[str, list[list[tuple[int, int]]]]]:
    stops_vec = NodeVector()
    stop_mapping = {}
    for i, (stop_id, stop) in enumerate(stops.items()):
        stop_mapping[stop_id] = i
        node = Node()
        node.loc = Coord(stop.lon, stop.lat)
        stops_vec.append(node)

    conns_vec = ConnectionVector()
    conn_mapping = {}
    schedules: dict[str, list[list[tuple[int, int]]]] = {
        "monday": [],
        "tuesday": [],
        "wednesday": [],
        "thursday": [],
        "friday": [],
        "saturday": [],
        "sunday": [],
    }
    for trip_id, trip in trips.items():
        if trip.service_id == -1 or trip.route_id == -1:
            continue
        route_id = trip.route_id
        service = services[trip.service_id]
        days = service.get_days()
        trip_stops = trip.stops
        for i in range(len(trip_stops)-1):
            curr_t_stop = trip_stops[i]
            next_t_stop = trip_stops[i+1]
            stop_a = stop_mapping[curr_t_stop.stop_id]
            stop_b = stop_mapping[next_t_stop.stop_id]
            dep = curr_t_stop.departure
            arr = next_t_stop.arrival
            if (stop_a, stop_b, route_id) not in conn_mapping:
                conn = Connection(stop_a, stop_b, route_id)
                conns_vec.append(conn)
                conn_id = len(conns_vec) - 1
                conn_mapping[(stop_a, stop_b, route_id)] = conn_id
                schedules["monday"].append([])
                schedules["tuesday"].append([])
                schedules["wednesday"].append([])
                schedules["thursday"].append([])
                schedules["friday"].append([])
                schedules["saturday"].append([])
                schedules["sunday"].append([])
            else:
                conn_id = conn_mapping[(stop_a, stop_b, route_id)]
            for day in days:
                sc: list[list[tuple[int, int]]]
                match day:
                    case 1:
                        sc = schedules["monday"]
                    case 2:
                        sc = schedules["tuesday"]
                    case 3:
                        sc = schedules["wednesday"]
                    case 4:
                        sc = schedules["thursday"]
                    case 5:
                        sc = schedules["friday"]
                    case 6:
                        sc = schedules["saturday"]
                    case 7:
                        sc = schedules["sunday"]
                    case _:
                        raise Exception(f"Invalid day: {day}")
                sc[conn_id].append((dep, arr))
    
    return stops_vec, conns_vec, schedules

def parse_gtfs(gtfs_path: str, filter_polygon: str) -> tuple[NodeVector, ConnectionVector, dict[str, list[list[tuple[int, int]]]]]:
    filter = None
    with open(filter_polygon, "r") as file:
        data = json.loads(file.read())
        features = data["features"]
        coords = features[0]["geometry"]["coordinates"]
        filter = Polygon(coords[0], coords[1:])

    stops = read_stop_locations(gtfs_path, filter)
    services = read_calendar(gtfs_path)
    trips = read_trips(gtfs_path, stops, services)

    return build_transit_graph(trips, stops, services)

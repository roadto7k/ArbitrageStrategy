import asyncio
from collections import deque

# Global dictionary to store market data
market_data = {}

# Lists to store triangles and their relationship to currency pairs
triangle_list = []
pair_to_triangles = {}

# Profit threshold to trigger a trading signal
profit_threshold = 0.001

# Queue to handle updates for currency pairs
update_queue = deque()

pairs_list = ["BTCETH", "ETHUSD", "USDBTC", "ETHSOL", "SOLBTC"] # pairs we watch
wallet_currencies = ['BTC', 'ETH'] # What we have in our wallet ready to transfer



def handle_market_data_update(pair, data):
    """
    Handle the market data update by updating the market data dictionary and adding the pair to the update queue.

    Parameters:
    - pair: The currency pair that has been updated.
    - data: The new market data for that pair.
    """
    market_data[pair] = data
    update_queue.append(pair)


def generate_triangles(pairs_list, wallet_currencies=None):
    """
    Generate all possible arbitrage triangles from the given list of currency pairs.
    For each pair, consider both 'bid' and 'ask' directions.
    Return a list of unique triangles, where each triangle is a list of (pair_name, 'bid' or 'ask').
    Optionally, only include triangles that contain at least one currency from wallet_currencies.
    For duplicates (same set of pairs), prioritize triangles that start with a wallet currency and have the most 'ask' sides.

    Parameters:
    - pairs_list: List of available currency pairs.
    - wallet_currencies: List of currencies held in the wallet (optional).

    Returns:
    - A list of unique arbitrage triangles.
    """
    # Remove duplicate pairs (e.g., BTCETH and ETHBTC are considered identical)
    cleaned_pairs = set()
    for pair in pairs_list:
        base_currency, quote_currency = pair[:3], pair[3:]
        if (quote_currency + base_currency) not in cleaned_pairs:
            cleaned_pairs.add(pair)

    # Build a list of edges considering both bid and ask sides
    edges = []
    for pair in cleaned_pairs:
        base_currency, quote_currency = pair[:3], pair[3:]
        # Edge for 'ask' direction: from base_currency to quote_currency
        edges.append({'from': base_currency, 'to': quote_currency, 'pair': pair, 'side': 'ask'})
        # Edge for 'bid' direction: from quote_currency to base_currency
        edges.append({'from': quote_currency, 'to': base_currency, 'pair': pair, 'side': 'bid'})

    # Generate all possible triangles
    unique_triangles = {}
    for edge1 in edges:
        A = edge1['from']
        B = edge1['to']
        pair1 = edge1['pair']
        side1 = edge1['side']
        for edge2 in edges:
            if edge2['pair'] == pair1 or edge2['from'] != B:
                continue  # Skip if the same pair is used or currencies do not connect
            C = edge2['to']
            pair2 = edge2['pair']
            side2 = edge2['side']
            for edge3 in edges:
                if edge3['pair'] in {pair1, pair2} or edge3['from'] != C or edge3['to'] != A:
                    continue  # Skip if the same pair is used or does not complete the triangle
                pair3 = edge3['pair']
                side3 = edge3['side']
                # Triangle found
                triangle = [
                    (pair1, side1),
                    (pair2, side2),
                    (pair3, side3)
                ]
                # Collect the currencies in the triangle
                currencies_in_triangle = {A, B, C}
                # Optionally filter triangles that contain at least one wallet currency
                if wallet_currencies is not None and not currencies_in_triangle.intersection(wallet_currencies):
                    continue  # Skip if no wallet currencies are in the triangle
                # Create a key based on the set of pairs used to identify duplicates
                key = frozenset([pair1, pair2, pair3])
                # Compute the number of 'ask' sides
                num_ask = sum(1 for p in triangle if p[1] == 'ask')
                # Check if triangle starts with a wallet currency
                starts_with_wallet_currency = A in wallet_currencies if wallet_currencies else False
                wallet_currency_priority = wallet_currencies.index(
                    A) if wallet_currencies and A in wallet_currencies else float('inf')
                # Now, check for duplicates
                if key not in unique_triangles:
                    # Add to unique triangles
                    unique_triangles[key] = {
                        'triangle': triangle,
                        'num_ask': num_ask,
                        'starts_with_wallet_currency': starts_with_wallet_currency,
                        'wallet_currency_priority': wallet_currency_priority
                    }
                else:
                    # Compare with existing triangle
                    existing = unique_triangles[key]
                    # First preference: triangle that starts with wallet currency
                    if starts_with_wallet_currency and not existing['starts_with_wallet_currency']:
                        unique_triangles[key] = {
                            'triangle': triangle,
                            'num_ask': num_ask,
                            'starts_with_wallet_currency': starts_with_wallet_currency,
                            'wallet_currency_priority': wallet_currency_priority
                        }
                    elif starts_with_wallet_currency == existing['starts_with_wallet_currency']:
                        # Compare wallet currency priority
                        if wallet_currency_priority < existing['wallet_currency_priority']:
                            unique_triangles[key] = {
                                'triangle': triangle,
                                'num_ask': num_ask,
                                'starts_with_wallet_currency': starts_with_wallet_currency,
                                'wallet_currency_priority': wallet_currency_priority
                            }
                        elif wallet_currency_priority == existing['wallet_currency_priority']:
                            # Compare number of 'ask' sides
                            if num_ask > existing['num_ask']:
                                unique_triangles[key] = {
                                    'triangle': triangle,
                                    'num_ask': num_ask,
                                    'starts_with_wallet_currency': starts_with_wallet_currency,
                                    'wallet_currency_priority': wallet_currency_priority
                                }
    # Now extract the triangles from unique_triangles
    result_triangles = [entry['triangle'] for entry in unique_triangles.values()]

    return result_triangles


def initialize_triangles(pairs_list, wallet_currencies=None):
    """
    Initialize the triangles and map them to their corresponding currency pairs.

    Parameters:
    - pairs_list: List of available currency pairs.
    - wallet_currencies: List of currencies held in the wallet (optional).
    """
    global triangle_list, pair_to_triangles
    triangle_list = generate_triangles(pairs_list, wallet_currencies)
    pair_to_triangles = {}
    for triangle in triangle_list:
        for pair, _ in triangle:
            if pair not in pair_to_triangles:
                pair_to_triangles[pair] = []
            pair_to_triangles[pair].append(triangle)


def is_triangle_valid(triangle):
    """
    Check if all pairs in the triangle are present in the market data.

    Parameters:
    - triangle: The triangle to validate.

    Returns:
    - True if the triangle is valid (i.e., all pairs are in market_data), False otherwise.
    """
    return all(pair in market_data for pair, _ in triangle)


def calculate_arbitrage_opportunity(triangle):
    """
    Calculate the profit from an arbitrage opportunity across a series of currency pairs.

    Parameters:
    - triangle: List of tuples containing currency pairs and order type ('bid' or 'ask').

    Returns:
    - profit: The resulting profit multiplier from the arbitrage cycle. A value greater than 1 indicates a potential arbitrage opportunity.
    """
    profit = 1.0
    for pair, order_type in triangle:
        # Retrieve the price for the pair
        price = market_data[pair]
        # Multiply for 'bid' orders (buying at the bid price), divide for 'ask' orders (selling at the ask price)
        if order_type == 'ask':
            profit /= price
        else:
            profit *= price
    return profit


def generate_trade_signal(triangle):
    """
    Generate a trade signal for a given triangle by adjusting pairs if necessary.
    If the order type is 'bid', the pair is reversed.

    Parameters:
    - triangle: The arbitrage triangle for which to generate a trading signal.

    Returns:
    - A list of adjusted pairs to be used for trading.
    """
    adjusted_pairs = []
    for pair, side in triangle:
        if side == 'bid':
            # Reverse the pair if side is 'bid'
            base_currency, quote_currency = pair[:3], pair[3:]
            adjusted_pairs.append(quote_currency + base_currency)
        else:
            adjusted_pairs.append(pair)
    return adjusted_pairs


def process_updates():
    """
    Process all currency pair updates from the queue and check for arbitrage opportunities.
    If an opportunity with profit greater than the threshold is found, generate a trade signal.
    """
    while update_queue:
        pair = update_queue.popleft()
        affected_triangles = pair_to_triangles.get(pair, [])
        for triangle in affected_triangles:
            if is_triangle_valid(triangle):
                profit = calculate_arbitrage_opportunity(triangle)
                if profit > profit_threshold:
                    trade_signal = generate_trade_signal(triangle)
                    # Here you can execute the trade or log the trade signal


async def main():
    """
    Main function to initialize triangles and continuously process market data updates.
    """
    initialize_triangles(pairs_list, wallet_currencies)
    while True:
        pair, data = await receive_data_from_websocket()
        handle_market_data_update(pair, data)
        process_updates()

if __name__ == "__main__":
    asyncio.run(main())

package com.al3aabna.tarneeb.entry;

public class Game implements Comparable<Game> {

	public String userId;
	public String userName;
	public int win;
	public int lose;

	public Game() {
		this(null, null, 0, 0);
	}

	public Game(String userId, int win, int lose) {
		this(userId, null, win, lose);
	}

	public Game(String userId, String userName, int win, int lose) {
		this.userId = userId;
		this.userName = userName;
		this.win = win;
		this.lose = lose;
	}

	public Game(String userId, String userName, int fbScore) {
		this.userId = userId;
		this.userName = userName;
		setFacebookScore(fbScore);
	}

	@Override
	public int compareTo(Game another) {
		int selfScore = this.win * 10 - this.lose * 7;
		int anotherScore = another.win * 10 - another.lose * 7;
		return (selfScore > anotherScore) ? 1 : (selfScore == anotherScore) ? 0 : -1;
	}

	public void setFacebookScore(int score) {
		win = (score >> 16) & 0xFFFF;
		lose = score & 0xFFFF;
	}

	public int getFacebookScore() {
		return ((win & 0xFFFF) << 16) | (lose & 0xFFFF);
	}
}

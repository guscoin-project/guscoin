// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"

#include "arith_uint256.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);



    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "NY Times 01/JAN/2018 Trump Defends Mental Fitness Saying He is a Stable Genius";
    const CScript genesisOutputScript = CScript() << ParseHex("0409442336e1c57cf0ddad15b2eca4deba85ddd13c13d52234bf1da8e618bb54d4000f5f40e8407073b2f05fa986ae2e4c1feca9e3f9e4f658bc8f3df5eb124b76") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP34Height = 710000;
        consensus.BIP34Hash = uint256S("fa09d204a83a768ed5a7c8d441fa62f2043abf420cff1226c7b4329aeb9d51cf");
        consensus.BIP65Height = 918684; // bab3041e8977e0dc3eeff63fe707b92bde1dd449d8efafb248c27c8264cc311a
        consensus.BIP66Height = 811879; // 7aceee012833fa8952f8835d8b1b3ae233cd6ab08fdb27a771d2bd7bdc491894
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 10 * 30; // 3.5 days
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6048; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8064; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000ba50a60f8b56c7fe0");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x9ff8aec1244f9cb6ce7a077983c9c2ce119808cc9ffa316be1bb3b3c5a4c7150"); //1259849

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
		pchMessageStart[0] = 0xa7;
		pchMessageStart[1] = 0xab;
		pchMessageStart[2] = 0x27;
		pchMessageStart[3] = 0xd6;
        nDefaultPort = 28333;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1515315613, 201873, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();


		/*arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
		
		consensus.hashGenesisBlock = uint256S("0x01");
		if (true && genesis.GetHash() != consensus.hashGenesisBlock)
		{
			LogPrintf("recalculating params for mainnet.\n");
			LogPrintf("old mainnet genesis nonce: %d\n", genesis.nNonce);
			LogPrintf("old mainnet genesis hash:  %s\n", consensus.hashGenesisBlock.ToString().c_str());
			// deliberately empty for loop finds nonce value.
			for (genesis.nNonce == 0; UintToArith256(genesis.GetPoWHash()) > hashTarget; genesis.nNonce++) {}
			LogPrintf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
			LogPrintf("new mainnet genesis nonce: %d\n", genesis.nNonce);
			LogPrintf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
		}*/

		assert(consensus.hashGenesisBlock == uint256S("0x9129d5e2e95570efcc7404d55f925539eee5ad1c8083fffeb8b5c9e30d7708b0"));
		assert(genesis.hashMerkleRoot == uint256S("0x211c9a816143aa151e5836230d09f6b8462b571f245606564f4b1b77e49b8bb8"));

        // Note that of those with the service bits flag, most only support a subset of possible options
		

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,16);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,204);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

		vFixedSeeds.clear();
		vSeeds.clear();

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {  0, uint256S("0x9129d5e2e95570efcc7404d55f925539eee5ad1c8083fffeb8b5c9e30d7708b0")}
            }
        };

		chainTxData = ChainTxData{
			// Data as of block db42d00d824950a125f9b08b6b6c282c484781562fa8b3bd29d6ce4a2627c348 (height 1259851).
			1515315613, // * UNIX timestamp of last known number of transactions
			0,//11428845,  // * total number of transactions between genesis and that timestamp
			500//   (the tx=... number in the SetBestChain debug.log lines)
			   //0.06     // * estimated number of transactions per second after that timestamp
		};
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP34Height = 76;
        consensus.BIP34Hash = uint256S("8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573");
        consensus.BIP65Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000364b0cbc3568");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xad8ff6c2f5580d2b50bd881e11312425ea84fa99f322bf132beb722f97971bba"); //153490

        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xd2;
        pchMessageStart[2] = 0xc8;
        pchMessageStart[3] = 0xf1;
        nDefaultPort = 29335;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1515315613, 201873, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();


		/*arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
		consensus.hashGenesisBlock = uint256S("0x00000bf9285c0a1be0e2673a21714847e4179f1d9faaaea1bd60ff4874adade0");
		if (false && genesis.GetHash() != consensus.hashGenesisBlock)
		{
			LogPrintf("recalculating params for testnet.\n");
			LogPrintf("old testnet genesis nonce: %d\n", genesis.nNonce);
			LogPrintf("old testnet genesis hash:  %s\n", consensus.hashGenesisBlock.ToString().c_str());
			// deliberately empty for loop finds nonce value.
			for (genesis.nNonce == 0; UintToArith256(genesis.GetHash()) > hashTarget; genesis.nNonce++) {}
			LogPrintf("new testnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
			LogPrintf("new testnet genesis nonce: %d\n", genesis.nNonce);
			LogPrintf("new testnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
		}*/

		assert(consensus.hashGenesisBlock == uint256S("0x9129d5e2e95570efcc7404d55f925539eee5ad1c8083fffeb8b5c9e30d7708b0"));
		assert(genesis.hashMerkleRoot == uint256S("0x211c9a816143aa151e5836230d09f6b8462b571f245606564f4b1b77e49b8bb8"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seed.guscointools.com", true);
        vSeeds.emplace_back("seed-b.guscoin.loshan.co.uk", true);
        vSeeds.emplace_back("dnsseed-testnet.thrasher.io", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x9129d5e2e95570efcc7404d55f925539eee5ad1c8083fffeb8b5c9e30d7708b0")},
            }
        };

		chainTxData = ChainTxData{
			// Data as of block db42d00d824950a125f9b08b6b6c282c484781562fa8b3bd29d6ce4a2627c348 (height 1259851).
			1515315613, // * UNIX timestamp of last known number of transactions
			1,//11428845,  // * total number of transactions between genesis and that timestamp
			 28 //   (the tx=... number in the SetBestChain debug.log lines)
			  //0.06     // * estimated number of transactions per second after that timestamp
		};

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 1 * 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xa8;
        pchMessageStart[1] = 0xac;
        pchMessageStart[2] = 0x28;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 29444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1515315613, 0, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
		arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
		consensus.hashGenesisBlock = uint256S("0x00000bf9285c0a1be0e2673a21714847e4179f1d9faaaea1bd60ff4874adade0");
		if (true && genesis.GetHash() != consensus.hashGenesisBlock)
		{
			LogPrintf("recalculating params for regres.\n");
			LogPrintf("old regres genesis nonce: %d\n", genesis.nNonce);
			LogPrintf("old regres genesis hash:  %s\n", consensus.hashGenesisBlock.ToString().c_str());
			// deliberately empty for loop finds nonce value.
			for (genesis.nNonce == 0; UintToArith256(genesis.GetHash()) > hashTarget; genesis.nNonce++) {}
			LogPrintf("new regres genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
			LogPrintf("new regres genesis nonce: %d\n", genesis.nNonce);
			LogPrintf("new regres genesis hash: %s\n", genesis.GetHash().ToString().c_str());
		}
		assert(consensus.hashGenesisBlock == uint256S("0x9129d5e2e95570efcc7404d55f925539eee5ad1c8083fffeb8b5c9e30d7708b0"));
		assert(genesis.hashMerkleRoot == uint256S("0x211c9a816143aa151e5836230d09f6b8462b571f245606564f4b1b77e49b8bb8"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x9129d5e2e95570efcc7404d55f925539eee5ad1c8083fffeb8b5c9e30d7708b0")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
